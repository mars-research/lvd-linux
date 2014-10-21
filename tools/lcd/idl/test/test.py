#!/usr/bin/env python3

from os import getcwd, listdir
from os.path import join
from subprocess import call, check_output

def main():
    print("in main")
    test_dir = getcwd()
    comp_dir = "../compiler"
    out_dir = join(test_dir, "out")
    for file in listdir(test_dir):
        print("in loop: " + file)
        if file.endswith(".idl"):
            try:
                result = check_output([comp_dir, '-header', file], timeout=2)
                print("in try")
            except:
                print("in except")
                pass


if __name__ == '__main__':
    main()
            
