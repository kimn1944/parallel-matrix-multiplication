# parallel-matrix-multiplication
The following program demostrates a multithreaded application executing matrix multipilcation.

You can test the program by running make and then the executable file called main in the following fashion:  
>make  
>./main <number of threads> <filename>  

The execution time and the result can be found in the result.txt file.

You can also run the run_stats.sh script to get the average runtime of the program for different thread count. Sample below:  
>./run_stats.sh  

The statistics will be recorded in the stats.txt file. Be advised that running the script might take a significant amount of time as the program performs multiple runs with numerous thread counts. 

Authors:
Nikita Kim
