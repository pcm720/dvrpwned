#!/usr/bin/env python3
"""Build a single FR30 payload using the GCC/binutils toolchain.

Supports both C (.c) and GNU assembly (.S/.s/.asm) sources.
"""
import argparse
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
TOOLCHAIN = os.path.join(ROOT, 'tools', 'gcc-fr30-elf', 'bin')
DEFAULT_BUILD_DIR = os.path.join(ROOT, 'build')
LINKER_TEMPLATE = os.path.join(ROOT, 'linker', '131.ld')
INCLUDE_DIR = os.path.join(ROOT, 'include')


def find_tools():
    return {
        'gcc': os.path.join(TOOLCHAIN, 'fr30-elf-gcc'),
        'as': os.path.join(TOOLCHAIN, 'fr30-elf-as'),
        'ld': os.path.join(TOOLCHAIN, 'fr30-elf-ld'),
        'objcopy': os.path.join(TOOLCHAIN, 'fr30-elf-objcopy'),
    }


def build(source, name, vaddr, extra_objs=None, build_dir=DEFAULT_BUILD_DIR,
          include_dir=INCLUDE_DIR, linker_script=None):
    """Compile/assemble and link a payload, returning (bin_path, elf_path).

    linker_script may be a custom path; if omitted, the default template is used.
    In either case, the LOADADDR placeholder is replaced with the payload vaddr.
    """
    tools = find_tools()
    for key, path in tools.items():
        if not os.path.exists(path):
            raise FileNotFoundError(
                f'{key} not found at {path}; build/install the fr30-elf toolchain first'
            )

    os.makedirs(build_dir, exist_ok=True)
    out = os.path.join(build_dir, name)
    source = os.path.join(ROOT, source) if not os.path.isabs(source) else source
    ext = os.path.splitext(source)[1].lower()

    ld_template_path = linker_script if linker_script else LINKER_TEMPLATE
    ld_script = os.path.join(build_dir, f'{name}.ld')
    with open(ld_template_path) as f:
        template = f.read()
    with open(ld_script, 'w') as f:
        f.write(template.replace('LOADADDR', f'0x{vaddr:08x}'))

    if ext == '.c':
        cmd = [tools['gcc'], '-mno-lsim', '-O2', '-I', include_dir, '-c', '-o', f'{out}.o', source]
    elif ext in ('.s', '.asm', '.S'):
        cmd = [tools['as'], '-o', f'{out}.o', source]
    else:
        raise ValueError(f'unsupported source extension: {ext}')
    subprocess.run(cmd, check=True)

    objs = [f'{out}.o'] + list(extra_objs or [])
    subprocess.run([tools['ld'], '-T', ld_script, '-o', f'{out}.elf'] + objs, check=True)
    subprocess.run([tools['objcopy'], '-O', 'binary', f'{out}.elf', f'{out}.bin'], check=True)

    return f'{out}.bin', f'{out}.elf'


def main():
    p = argparse.ArgumentParser(description='Build an FR30 payload')
    p.add_argument('source', help='source file (.c or .S/.s/.asm)')
    p.add_argument('name', help='base name for build artifacts')
    p.add_argument('vaddr', help='payload load virtual address (hex)')
    p.add_argument('--extra-obj', action='append', default=[],
                   help='additional object files to link (can be repeated)')
    p.add_argument('--build-dir', default=DEFAULT_BUILD_DIR)
    p.add_argument('--linker-script', default=None,
                   help='custom linker script template (LOADADDR is replaced)')
    args = p.parse_args()
    linker_script = args.linker_script
    if linker_script and not os.path.isabs(linker_script):
        linker_script = os.path.join(ROOT, linker_script)
    build(args.source, args.name, int(args.vaddr, 0), extra_objs=args.extra_obj,
          build_dir=args.build_dir, linker_script=linker_script)


if __name__ == '__main__':
    main()
