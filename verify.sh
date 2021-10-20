#!/bin/bash
dir=./aoi_benchmark/
set -e

for file in $(ls $dir); do
    time ./a.out ./aoi_benchmark/$file
    echo /aoi_benchmark/$file sucess!
done