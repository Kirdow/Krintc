#!/bin/bash

CC="${CXX:-cc}"
CSTDVER="c11"
CFLAGS="-Wall -Wextra -std=${CSTDVER}"
CDEF=""
SRC="src/vendor/stb_impl.c src/mem.c src/ptrlist.c src/tests.c src/args.c src/files.c src/demo.c src/krintc.c src/point.c"
LDFLAGS="-lm"

mem_flag=false
memd_flag=false

for arg in "$@"
do
    if [[ $arg == "--mem" ]]; then
        mem_flag=true
    elif [[ $arg == "--memd" ]]; then
        mem_flag=true
        memd_flag=true
    fi
done

if [[ "$OSTYPE" == "msys" ]]; then
    CDEF="$CDEF -DWIN32"
fi

if $mem_flag; then
    CDEF="$CDEF -DMEM_WRAP"
fi

if $memd_flag; then
    CDEF="$CDEF -DMEM_DEBUG"
fi

if [[ ! -d target ]]; then
    mkdir target
fi

$CC $CFLAGS $CDEF -o target/demo $SRC $LDFLAGS

