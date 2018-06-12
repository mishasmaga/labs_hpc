#!/bin/sh
awk 'BEGIN {xmax=100; xmin=-100; step=2000; dx=(xmax-xmin)/step;  for (i = xmin; i < xmax; i+=dx) print i" "i+dx }' > ranges.txt
g++ integral.cxx -oexe
rm -rf res
mkdir res
rm -rf out
mkdir out
qsub ./build_job.pbs -t 1-10