#!/usr/bin/env python3
"""Main build entry point for the DVRP GCC-only firmware patcher.

Usage:
    python3 build.py manifests/ata_wrappers_c.json

The manifest must contain an `input_udm` field specifying the source firmware.
The patched, checksum-correct output is written to `output_udm`.
"""
import argparse
import json
import os
import subprocess
import sys
from pathlib import Path

ROOT = os.path.dirname(os.path.abspath(__file__))
PATCHER = os.path.join(ROOT, 'tools', 'scripts', 'patcher.py')
CHECKSUM_TOOL = os.path.join(ROOT, 'tools', 'scripts', 'fix_checksum.py')


def run(argv, **kwargs):
    print('>', ' '.join(argv))
    subprocess.run(argv, check=True, cwd=ROOT, **kwargs)


def main():
    p = argparse.ArgumentParser(description='Build a patched DVRP firmware from a manifest')
    p.add_argument('manifest', help='JSON manifest describing payloads and hooks')
    p.add_argument('-i', '--input-udm',
                   help='override input firmware (default: input_udm from manifest)')
    args = p.parse_args()

    manifest_path = args.manifest if os.path.isabs(args.manifest) else os.path.join(ROOT, args.manifest)
    with open(manifest_path) as f:
        manifest = json.load(f)

    input_udm = args.input_udm or manifest.get('input_udm')
    if not input_udm:
        raise SystemExit('manifest missing input_udm; supply --input-udm')

    output_udm = manifest.get('output_udm')
    if not output_udm:
        raise SystemExit('manifest missing output_udm')

    # Ensure output goes under build/ if the manifest uses a bare filename.
    if os.path.dirname(output_udm) == '':
        output_udm = os.path.join('build', output_udm)

    intermediate = os.path.join(ROOT, 'build', 'patched_' + os.path.basename(output_udm) + ".tmp")
    os.makedirs(os.path.dirname(intermediate), exist_ok=True)

    # 1. Apply patch (build payloads + hooks) to an intermediate file.
    patcher_args = [sys.executable, PATCHER, manifest_path, '--output-udm', intermediate]
    if args.input_udm:
        patcher_args.extend(['--input-udm', args.input_udm])
    run(patcher_args)

    # 2. Fix checksums.
    run([sys.executable, CHECKSUM_TOOL, 'fix', intermediate, output_udm])

    # 3. Verify checksums.
    run([sys.executable, CHECKSUM_TOOL, 'verify', output_udm])

    # 4. Cleanup
    extensions = (".o", ".bin", ".ld", ".elf", ".tmp")
    builddir = Path(os.path.join(ROOT, 'build'))
    if builddir.is_dir():
        for file in builddir.glob("*"):
            if file.is_file() and file.suffix.lower() in extensions:
                file.unlink()

if __name__ == '__main__':
    main()
