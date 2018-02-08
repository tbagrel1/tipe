#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""Recursively compress a file with a zip algorithm."""

import os
import math
import json

# .format(dest_name_with_ext, src_name_with_ext)
ZIP_CMD = "time zip -q -9 {} {}"
# .format(src_name_with_ext)
UNZIP_CMD = "time unzip -q -o {}"
# .format(name_with_ext)
SPACE_CMD = "du -b {}"
INIT_FILE = "calgary.tar"
RESULT_FILE = "rec_zip_measures.txt"

PASSES = 10
N = int(math.log(PASSES, 10)) + 1

def make_compress_pass(n):
    """Makes the nth pass of the compression."""
    if n == 1:
        cmd = ZIP_CMD.format(INIT_FILE + "_{}.zip".format(n), INIT_FILE)
    else:
        cmd = ZIP_CMD.format(INIT_FILE + "_{}.zip".format(n),
                             INIT_FILE + "_{}.zip".format(n - 1))
    return os.popen(cmd).read()

def make_decompress_pass(n):
    """Makes the nth pass of the decompression."""
    return os.popen(
        UNZIP_CMD.format(INIT_FILE + "_{}.zip".format(n))).read()

def get_space(n):
    """Gets the space used by the nth pass file."""
    if n == 0:
        cmd = SPACE_CMD.format(INIT_FILE)
    else:
        cmd = SPACE_CMD.format(INIT_FILE + "_{}.zip".format(n))
    raw_space = os.popen(cmd).read()
    return int(raw_space.split("\t")[0])

def main():
    """Main function."""
    for i in range(1, PASSES + 1):
        make_compress_pass(i)
    for i in range(1, PASSES + 1):
        make_decompress_pass(i)
    spaces = []
    for i in range(0, PASSES + 1):
        spaces.append(get_space(i))
    with open(RESULT_FILE, "w", encoding="utf-8") as result:
        result.write(json.dumps(
            {
                "file": INIT_FILE,
                "spaces": spaces
            }
        ))

if __name__ == "__main__":
    main()

