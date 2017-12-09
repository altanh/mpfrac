#!/bin/bash
INCLUDE="-I/usr/local/Cellar/mpfr/3.1.6/include -I/usr/local/Cellar/libpng/1.6.25/include"
LIB="-L/usr/local/Cellar/mpfr/3.1.6/lib -L/usr/local/Cellar/libpng/1.6.25/lib -lmpfr -lpng"
OUT="mpfrac"
g++ -g -std=c++11 $INCLUDE $LIB src/*.cpp -o $OUT