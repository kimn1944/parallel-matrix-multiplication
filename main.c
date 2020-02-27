#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define  MASTER	    0

int main (int argc, char *argv[]) {
    int   numtasks, taskid;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

    MPI_Finalize();
}
