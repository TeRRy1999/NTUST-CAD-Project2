#!/bin/bash
dir=./aoi_benchmark/
set -e


for file in $(ls $dir); do
    echo ML-RCS running [given 10 10 10]
    time ./list -l ./aoi_benchmark/$file 10 10 10
    echo /aoi_benchmark/$file sucess!

    echo MR-LCS running [given 100]
    time ./list -r ./aoi_benchmark/$file 100
    echo /aoi_benchmark/$file sucess!


done