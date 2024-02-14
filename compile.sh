#!/bin/bash

gcc -Wall -static -O3 -c crycon.c
cd src/
gcc -Wall -static -O3 -c *.c
cp *.o ../
cd ..
gcc -Wall -static -O3 *.o -o crycon

