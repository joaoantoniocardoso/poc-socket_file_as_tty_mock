#!/bin/env sh

\rm -rf cli.sock main
gcc -Os -Wall -Wextra -std=c11 main.c -o main
./main
