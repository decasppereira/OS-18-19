#!/bin/bash

numThreads=$1

# Filenames
filename=$(basename $2)
input="../"$2
output="../results/"$filename".speedups.csv"
touch $output       # creates the output file
execTime=$input".res"

echo "#threads,exec_time,speedup" > $output

../CircuitRouter-SeqSolver/CircuitRouter-SeqSolver "$input">/dev/null
seqTime=$(cut -c 19-27 <(grep "Elapsed" $execTime))    # Duration of SeqSolver

echo "1S, $seqTime, 1" >> $output

# Speedup for each number of threads
for ((i = 1; i <= $numThreads; i++)); do
    ./CircuitRouter-ParSolver -t $numThreads $input >/dev/null
    parTime=$(cut -c 19-27 <(grep "Elapsed" $execTime))
    speedup=$(echo "scale=6; ${seqTime}/${parTime}" | bc)
    echo "$i, $parTime, $speedup" >> $output
done
