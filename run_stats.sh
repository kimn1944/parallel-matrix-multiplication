#!/bin/bash

filename="mat200x200.txt"
#filename="mat4000x4000.txt"
thread_max=5
run_num=5

make
echo "Statistics for the parallel matrix multiplication." > stats.txt
echo "Each calculation will be repeated $run_num times and with up to $thread_max threads" >> stats.txt

for ((i=1; i<=$thread_max; i++))
do 
    sum=0
    for ((j=1; j<=$run_num; j++))
    do
	START=$(date +%s)
	    ./main $i $filename 
	END=$(date +%s)
	let sum=$sum+$END-$START
    done
    echo "$i threaded application on average took $(echo "scale=4; $sum / $run_num" | bc) seconds." >> stats.txt
done

