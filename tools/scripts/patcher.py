#!/usr/bin/env python3
"""Manifest-driven UDM patcher using the GCC/binutils FR30 toolchain.

Does not recompute checksums; the caller is responsible for fixing them
after this script returns.
"""
import argparse
import json
import os
import struct
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
SEG1_VADDR = 0x10000000
SEG1_FILEOFF = 0x44


def vaddr_to_fileoff(vaddr):
    if SEG1_VADDR <= vaddr < SEG1_VADDR + 0x01000000:
        return SEG1_FILEOFF + (vaddr - SEG1_VADDR)
    raise ValueError(f'vaddr 0x{vaddr:08x} is outside known segment-1 range')


def parse_int(x):
    return int(x, 0) if isinstance(x, str) else int(x)


def build_payload(src, name, vaddr, linker_script=None):
    """Run tools/build/build_payload.py for a source file and return .bin path."""
    script = os.path.join(ROOT, 'tools', 'scripts', 'build_payload.py')
    if not os.access(script, os.X_OK):
        os.chmod(script, 0o755)
    cmd = [sys.executable, script, src, name, f'0x{vaddr:08x}']
    if linker_script:
        cmd.extend(['--linker-script', linker_script])
    subprocess.run(cmd, check=True, cwd=ROOT)
    bin_path = os.path.join(ROOT, 'build', name + '.bin')
    if not os.path.exists(bin_path):
        raise FileNotFoundError(f'expected {bin_path} after build')
    return bin_path


def find_symbol(name, src_path):
    """Resolve a symbol address from the built ELF file using fr30-elf-nm."""
    base = os.path.splitext(os.path.basename(src_path))[0]
    elf = os.path.join(ROOT, 'build', base + '.elf')
    nm = os.path.join(ROOT, 'tools', 'gcc-fr30-elf', 'bin', 'fr30-elf-nm')
    if not os.path.exists(elf):
        return None
    out = subprocess.run([nm, elf], check=True, capture_output=True, text=True)
    for line in out.stdout.splitlines():
        parts = line.split()
        if len(parts) >= 3 and parts[2] == name:
            return int(parts[0], 16)
    return None


def inject_bytes(data, payload, vaddr, label):
    """Inject raw bytes into the firmware image at vaddr, checking the region is unused."""
    fileoff = vaddr_to_fileoff(vaddr)
    end = fileoff + len(payload)
    region = data[fileoff:end]
    if not all(b == 0x00 or b == 0xff for b in region):
        raise RuntimeError(
            f'{label} region 0x{fileoff:x}-0x{end:x} is not unused: {region[:16].hex()}...')
    data[fileoff:end] = payload
    print(f'{label} 0x{len(payload):x} bytes at vaddr 0x{vaddr:08x}')


def apply_patch(udm_path, manifest_path, output_path=None):
    with open(manifest_path) as f:
        manifest = json.load(f)

    if udm_path is None:
        udm_path = manifest.get('input_udm')
        if udm_path is None:
            raise SystemExit('manifest missing input_udm; supply --input-udm')
    udm_path = os.path.join(ROOT, udm_path) if not os.path.isabs(udm_path) else udm_path

    with open(udm_path, 'rb') as f:
        data = bytearray(f.read())

    default_linker_script = manifest.get('linker_script')
    for p in manifest.get('payloads', []):
        src = p['source']
        name = os.path.splitext(os.path.basename(src))[0]
        vaddr = parse_int(p['vaddr'])
        linker_script = p.get('linker_script', default_linker_script)
        if linker_script and not os.path.isabs(linker_script):
            linker_script = os.path.join(ROOT, linker_script)
        bin_path = build_payload(src, name, vaddr, linker_script=linker_script)
        with open(bin_path, 'rb') as f:
            payload = f.read()
        inject_bytes(data, payload, vaddr, f'injected {src}:')

    for fspec in manifest.get('files', []):
        src = fspec['source']
        vaddr = parse_int(fspec['vaddr'])
        src_path = os.path.join(ROOT, src) if not os.path.isabs(src) else src
        with open(src_path, 'rb') as f:
            payload = f.read()
        inject_bytes(data, payload, vaddr, f'injected {src}:')

    for h in manifest.get('hooks', []):
        htype = h.get('type', 'raw')
        vaddr = parse_int(h['vaddr'])
        fileoff = vaddr_to_fileoff(vaddr)
        if htype == 'raw':
            new_bytes = bytes.fromhex(h['bytes'].replace(' ', ''))
        elif htype == 'call32_target':
            target = parse_int(h['target'])
            helper_symbol = h.get('helper_symbol')
            helper_payload = h.get('helper_payload')
            if helper_symbol and helper_payload:
                helper_addr = find_symbol(helper_symbol, helper_payload)
                if helper_addr is not None:
                    target = helper_addr
            prefix = data[fileoff:fileoff+2]
            if prefix != b'\x9f\x8c':
                print(f'warning: unexpected call32 prefix at 0x{vaddr:08x}: {prefix.hex()}',
                      file=sys.stderr)
            suffix = data[fileoff+6:fileoff+8]
            new_bytes = prefix + struct.pack('>I', target) + suffix
        else:
            raise ValueError(f'unknown hook type {htype}')

        if 'expected' in h:
            expected = bytes.fromhex(h['expected'].replace(' ', ''))
            actual = data[fileoff:fileoff+len(expected)]
            if actual != expected:
                raise RuntimeError(
                    f'hook at 0x{vaddr:08x}: expected {expected.hex()}, found {actual.hex()}')

        end = fileoff + len(new_bytes)
        data[fileoff:end] = new_bytes
        print(f'hook 0x{vaddr:08x}: {new_bytes.hex()}')

    out_path = output_path or manifest['output_udm']
    out_path = os.path.join(ROOT, out_path) if not os.path.isabs(out_path) else out_path
    os.makedirs(os.path.dirname(os.path.abspath(out_path)) or '.', exist_ok=True)
    with open(out_path, 'wb') as f:
        f.write(data)
    print(f'wrote {out_path}')


def main():
    p = argparse.ArgumentParser()
    p.add_argument('manifest')
    p.add_argument('-i', '--input-udm',
                   help='override input firmware (default: input_udm from manifest)')
    p.add_argument('-o', '--output-udm',
                   help='override output path (default: output_udm from manifest)')
    args = p.parse_args()
    apply_patch(args.input_udm, args.manifest, output_path=args.output_udm)


if __name__ == '__main__':
    main()
