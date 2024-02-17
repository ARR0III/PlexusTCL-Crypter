#!/bin/bash

gcc -Wall -O3 -c crycon.c
cd src/
clang -Wall -O3 -c *.c
cd ..
clang -Wall -static -O3 crycon.o src/*.o -o crycon

