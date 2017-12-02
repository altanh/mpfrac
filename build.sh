#!/bin/bash
g++ -g -std=c++11 -I/usr/local/Cellar/mpfr/3.1.6/include -L/usr/local/Cellar/mpfr/3.1.6/lib -lmpfr *.cpp -o mpfrac