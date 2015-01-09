#!/usr/bin/env python3

from os import getcwd, listdir
from os.path import join
from subprocess import check_call, check_output, CalledProcessError

def main():
    current_dir = getcwd()
    test_dir = join(current_dir, "test")
    comp_dir = join(current_dir, "compiler")
    out_dir = join(test_dir, "out")
    for file in listdir(test_dir):
        if file.endswith(".idl"):
            try:
                print("in loop: " + file)
                idl_file = join("test", file)
                out_file = join("test", file+".h")
                print(out_file)
                print(idl_file)
            
                check_call([comp_dir, '-header', idl_file, '-out', out_file])
            except Exception as e:
                print("f")
    
if __name__ == '__main__':
    main()
            
