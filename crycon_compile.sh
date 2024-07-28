#!/bin/bash

gcc -Wall -D_FILE_OFFSET_BITS=64 -O0 -c crycon_linux.c
cd src/
clang -Wall -O3 -c *.c
cd ..
clang -Wall -static src/*.o crycon_linux.o -o crycon
