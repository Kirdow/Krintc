#!/bin/bash

CC="${CXX:-cc}"
CSTDVER="c11"
CFLAGS="-Wall -Wextra -std=${CSTDVER}"
CDEF=""
SRC=$(echo "$(find src -name "*.c")" | sed ':a;N;$!ba;s/\n/ /g')
INCLUDES="-Isrc"
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

if [[ "$OSTYPE" == "darwin"* ]]; then
	CDEF="$CDEF -DMACOS"
fi

if [[ "$OSTYPE" == *"Linux"* || "$OSTYPE" == *"linux"* ]]; then
	CDEF="$CDEF -DLINUX"
fi

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

CMD="$CC $CFLAGS $CDEF $INCLUDES -o target/demo $SRC $LDFLAGS"
echo "$CMD"
$CMD


