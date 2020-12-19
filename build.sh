#!/bin/bash
gcc lex.c compiler.c parse.c -o crc
./crc casm/test2.crc > a
cat a
echo "-----"
casm a a > b
casm r b

rm -rf a
rm -rf b
