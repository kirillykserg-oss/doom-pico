#!/usr/bin/env python3
"""Склеює прошивку Doom (.bin) і рівні (doom1.whx) в ОДИН .uf2 для RP2040.

Використання:
    python make_uf2.py doom_tiny.bin doom1.whx DOOM.uf2 [адреса_WAD]

За замовчуванням WAD лежить за адресою 0x10042000 (має збігатися з
TINY_WAD_ADDR у src/CMakeLists.txt). Прошивка кладеться з 0x10000000.
Можна передати і .uf2 замість .bin (тоді з нього береться вміст).
"""
import struct, sys

FLASH_BASE = 0x10000000
FAMILY_RP2040 = 0xE48BFF56
MAGIC0, MAGIC1, MAGIC_END = 0x0A324655, 0x9E5D5157, 0x0AB16F30

def read_uf2_as_bin(path):
    data = open(path, "rb").read()
    blocks = []
    for i in range(0, len(data), 512):
        b = data[i:i + 512]
        m0, m1, flags, addr, size, no, total, fam = struct.unpack("<8I", b[:32])
        assert m0 == MAGIC0 and m1 == MAGIC1, "це не UF2"
        blocks.append((addr, b[32:32 + size]))
    blocks.sort()
    base = blocks[0][0]
    out = bytearray()
    for addr, payload in blocks:
        off = addr - base
        if len(out) < off:
            out += b"\xff" * (off - len(out))
        out[off:off + len(payload)] = payload
    assert base == FLASH_BASE, "UF2 має починатися з 0x10000000"
    return bytes(out)

def write_uf2(image, path, base=FLASH_BASE):
    chunks = [image[i:i + 256] for i in range(0, len(image), 256)]
    total = len(chunks)
    with open(path, "wb") as f:
        for n, c in enumerate(chunks):
            c = c.ljust(256, b"\xff")
            hdr = struct.pack("<8I", MAGIC0, MAGIC1, 0x00002000,
                              base + n * 256, 256, n, total, FAMILY_RP2040)
            f.write(hdr + c.ljust(476, b"\x00") + struct.pack("<I", MAGIC_END))

def main():
    if len(sys.argv) < 4:
        print(__doc__); sys.exit(1)
    fw_path, wad_path, out = sys.argv[1:4]
    wad_addr = int(sys.argv[4], 0) if len(sys.argv) > 4 else 0x10042000
    fw = read_uf2_as_bin(fw_path) if fw_path.lower().endswith(".uf2") else open(fw_path, "rb").read()
    wad = open(wad_path, "rb").read()
    off = wad_addr - FLASH_BASE
    if len(fw) > off:
        sys.exit(f"ПОМИЛКА: прошивка ({len(fw):#x} байт) заходить на область WAD ({off:#x}). "
                 "Збільш адресу WAD і TINY_WAD_ADDR у src/CMakeLists.txt.")
    if off + len(wad) > 2 * 1024 * 1024:
        sys.exit("ПОМИЛКА: прошивка + WAD не вміщуються у 2 МБ флеш-пам'яті Pico.")
    image = fw.ljust(off, b"\xff") + wad
    write_uf2(image, out)
    print(f"OK: {out}  (прошивка {len(fw)} Б, WAD {len(wad)} Б за {wad_addr:#x}, всього {len(image)} Б)")

if __name__ == "__main__":
    main()
