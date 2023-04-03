#!/bin/sh

CC="${CXX:-cc}"
CFLAGS="-Wall -Wextra -std=c11"
SRC="demo.c krintc.c point.c"


$CC $CFLAGS -o demo $SRC
