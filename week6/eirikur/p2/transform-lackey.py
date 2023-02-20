#!/usr/bin/env python3

# Put your script here (instead of the print statements)
# Your script shall read the valgrind output on stdin,
# and produce a list of newline-separated lines of virtual page numbers


import sys

PAGE_SIZE = 4096

for line in sys.stdin:
    if line.startswith("=="):
        continue
    else:
        parts = line.split()
        virtual_address, size = parts[1].split(',')
        virtual_address = int(virtual_address, 16)
        virtual_page = virtual_address // PAGE_SIZE

        sys.stdout.write(f"{virtual_page}\n")