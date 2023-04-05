#!/bin/bash

if [[ ! -d run ]]; then
    mkdir run
fi

EXEC="demo"

if [[ "$OSTYPE" == "msys" ]]; then
    EXEC="demo.exe"
fi

cd run
./../target/$EXEC --testdir ../tests "$@"
cd ..
