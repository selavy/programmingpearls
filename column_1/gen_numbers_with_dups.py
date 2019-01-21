#!/usr/bin/env python

import sys
import numpy as np


if __name__ == '__main__':
    M = 10000000
    if len(sys.argv) < 2:
        N = 1000000
    elif len(sys.argv) == 2:
        N = int(sys.argv[1])
    else:
        sys.stderr.write(f"Usage: {sys.argv[0]} <NUM>\n")
        sys.exit(0)

    if N > M:
        raise Exception(f"{N} must be lower than {M}")

    N = N // 10
    for iteration in range(10):
        vals = np.random.choice(range(M), N, replace=False)
        for i in vals:
            print(i)

