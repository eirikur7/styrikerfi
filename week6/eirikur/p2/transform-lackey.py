#!/usr/bin/env python3

# Put your script here (instead of the print statements)
# Your script shall read the valgrind output on stdin,
# and produce a list of newline-separated lines of virtual page numbers

# import sys
# import re

# for line in sys.stdin:
#     # print("line: " + line)
#     m = re.search(' ([0-9a-fA-F]+)[0-9a-fA-F]{3},', line)
#     # print(f"M:    {m}")
#     if m:
#         print(int(m.group(1),16) // 4096)

# #!/usr/bin/env python3

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