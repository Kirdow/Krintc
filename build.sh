#!/bin/sh

CC="${CXX:-cc}"
CSTDVER="c11"
CFLAGS="-Wall -Wextra -std=${CSTDVER}"
CDEF=""
SRC="src/vendor/stb_impl.c src/ptrlist.c src/tests.c src/args.c src/files.c src/demo.c src/krintc.c src/point.c"

if [[ "$OSTYPE" == "msys" ]]; then
    CDEF="$CDEF -DWIN32"
fi

if [[ ! -d target ]]; then
    mkdir target
fi

$CC $CFLAGS $CDEF -o target/demo $SRC
