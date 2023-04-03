#!/bin/sh

CC="${CXX:-cc}"
CSTDVER="c11"
CFLAGS="-Wall -Wextra -std=${CSTDVER}"
SRC="src/demo.c src/krintc.c src/point.c"

mkdir target
$CC $CFLAGS -o target/demo $SRC
