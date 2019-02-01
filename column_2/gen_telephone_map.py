#!/usr/bin/env python

import pprint
import sys

vals = {
    'A': '2', 'B': '2', 'C': '2', 'D': '3', 'E': '3', 'F': '3', 'G': '4',
    'H': '4', 'I': '4', 'J': '5', 'K': '5', 'L': '5', 'M': '6', 'N': '6',
    'O': '6', 'P': '7', 'Q': '7', 'R': '7', 'S': '7', 'T': '8', 'U': '8',
    'V': '8', 'W': '9', 'X': '9', 'Y': '9', 'Z': '9',
}

sys.stdout.write("const char ButtonMap[64] = {")
tbl = [vals.get(chr(x).upper(), '0') for x in range(64, 128)]
# tbl = [f"'{x}'" if x != 0 else '0' for x in tbl]
for i, x in enumerate(tbl):
    if i % 8 == 0:
        sys.stdout.write("\n    ")
    sys.stdout.write(f"'{x}', ")

# for i in range(64, 128):
#     val = vals.get(chr(i).upper(), 0)
#     print(f"'{val}',")
sys.stdout.write("\n};\n")
