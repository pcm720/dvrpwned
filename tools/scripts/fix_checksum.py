#!/usr/bin/env python3
"""
DVRP 1.31 firmware (.udm) container checksum tool.

Container layout (big-endian) as reverse-engineered from DVRP_FIRMWARE.udm:

  0x00  'DVRP'  magic
  0x04  0x00000018   main header length (24)
  0x08  version (0x00010209)
  0x0C  image count (1)
  0x10  payload length (= filelen - 24)
  0x14  PAYLOAD CHECKSUM  = sum32be(file[0x18:end])          <-- cracked
  --- image record ---
  0x18  0x14   image-record header len
  0x1C  0
  0x20  0
  0x24  image body length (0x7b000)
  0x28  BODY CHECKSUM h1  = sum32be(file[0x2C:0x2C+0x7b000]) <-- cracked
  0x2C  h2                = <non-standard, not yet cracked>
  --- segment 1 descriptor ---
  0x30  0x18
  0x34  0x10000000   load address (FR60 vaddr base)
  0x38  0x0003ffe8   segment length
  0x3C  SEG1 CHECKSUM     = sum32be(file[0x44:0x44+0x3ffe8]) <-- cracked
  0x40  ...          segment-1 code/data begins here
                     (FR60 code: file 0x44 == vaddr 0x10000000)
  0x4002C ...        segment-2 header (magic-less: crc,version,...) + data

sum32be(range) = sum of the range interpreted as big-endian uint32 words, mod 2**32.

NOTE: h2 @0x2C is NOT reproduced by this tool. It is not needed in this instance..
"""
import struct, sys

def sum32be(b):
    b = b[:len(b)//4*4]
    return sum(struct.unpack(">%dI" % (len(b)//4), b)) & 0xFFFFFFFF

def rd32(d, o): return struct.unpack(">I", d[o:o+4])[0]

def verify(d):
    seg_len = rd32(d, 0x38)
    checks = [
        ("seg1 @0x3C", 0x3C, sum32be(d[0x44:0x44+seg_len])),
        ("body h1 @0x28", 0x28, sum32be(d[0x2C:0x2C + rd32(d, 0x24)])),
        ("payload @0x14", 0x14, sum32be(d[0x18:])),
    ]
    print(f"magic={d[0:4]!r}  filelen=0x{len(d):x}  seglen=0x{seg_len:x}")
    for name, off, calc in checks:
        stored = rd32(d, off)
        ok = "OK" if stored == calc else "MISMATCH"
        print(f"  {name:14} stored=0x{stored:08x} calc=0x{calc:08x}  [{ok}]")
    print(f"  h2 @0x2C       stored=0x{rd32(d,0x2C):08x}  [NOT VERIFIED - unknown algo]")

def fix(d):
    """Recompute the three cracked checksums in the correct dependency order.
    Order matters: seg1(0x3C) is inside body(0x28) is inside payload(0x14)."""
    d = bytearray(d)
    seg_len = rd32(d, 0x38)
    # 1) segment-1 checksum
    struct.pack_into(">I", d, 0x3C, sum32be(d[0x44:0x44+seg_len]))
    # 2) body h1 (covers 0x2C..end, which includes 0x3C we just fixed)
    struct.pack_into(">I", d, 0x28, sum32be(d[0x2C:0x2C + rd32(d, 0x24)]))
    # 3) payload checksum (covers 0x18..end, includes 0x28 and 0x3C)
    struct.pack_into(">I", d, 0x14, sum32be(d[0x18:]))
    return bytes(d)

if __name__ == "__main__":
    if len(sys.argv) < 3 or sys.argv[1] not in ("verify", "fix"):
        print("usage: dvrp_checksum.py verify <file>")
        print("       dvrp_checksum.py fix <in_file> <out_file>")
        sys.exit(1)
    d = open(sys.argv[2], "rb").read()
    if sys.argv[1] == "verify":
        verify(d)
    else:
        out = fix(d)
        open(sys.argv[3], "wb").write(out)
        print(f"wrote {sys.argv[3]}")
        verify(out)
