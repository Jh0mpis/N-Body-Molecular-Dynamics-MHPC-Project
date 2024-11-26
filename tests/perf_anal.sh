#! /bin/bash

cd ../examples
sudo perf stat -e cycles,instructions,cache-references,cache-misses -o perf_my_code.txt ../build/bin/main.x < argon_108.inp
