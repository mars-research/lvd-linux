#! /usr/bin/env python

import os
import sys

print os.path.relpath(sys.argv[1], sys.argv[2])
