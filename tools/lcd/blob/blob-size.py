# quick script for calc of final blob size with padding to proper number
# of pages

import sys
import shutil
import math
import os

if len(sys.argv) != 3:
    sys.stderr.write("blob-size.py: usage: python blob-size.py <blob fname> -{s,o}\n    use s for size in bytes, o for order (the output file will be 2^o pages)")
    sys.exit(-1)

bfname = sys.argv[1]
npages = (os.path.getsize(bfname) + 4095) / 4096
order = int(math.log(npages, 2))
if 2 ** order != npages:
    order += 1

if sys.argv[2] == "-s":
    sys.stdout.write(str((2 ** order) * 4096))
elif sys.argv[2] == "-o":
    sys.stdout.write(str(order))
else:
    sys.stderr.write("blob-size.py: unexpected second option " + sys.argv[2] + "\n")
    exit(-1)
