#!/bin/sh

if [[ ! -d run ]]; then
    mkdir run
fi

cd run
./../target/demo --testdir ../tests "$@"
cd ..
