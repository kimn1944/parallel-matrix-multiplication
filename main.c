#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define MASTER 0
#define FROM_MASTER 0
#define	FROM_WORKER 1

int *MAT_PTR = NULL;
int *RESULT = NULL;
int ROW = 0;
int COL = 0;

void io(int argc, char **argv) {
    if (argc < 3) {
	printf("Usage: %s <number of threads> <name of file>\n", argv[0]);
	exit(0);
    }
    char *filename = argv[2];
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) { 
	printf("Cannot open file\n");
	exit(0);
    }
    fscanf(fptr, "%d", &ROW);
    fscanf(fptr, "%d", &COL);
    int size = ROW * COL;
    MAT_PTR = (int*)malloc(size * sizeof(int)); 
    RESULT = (int*)malloc(size * sizeof(int));
    int i, j;
    for (i = 0; i < ROW; i++) {
	for (j = 0; j < COL; j++) {
	    fscanf(fptr, "%d", MAT_PTR + i * COL + j);
	} 
    }
    fclose(fptr);
}

int main(int argc, char *argv[]) {
    int numtasks, taskid, source, destination, msg_type; // MPI stuff
    int rows, average, modulo, offset;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    
    if (taskid == MASTER) {
	io(argc, argv);
	average = COL / numtasks;
	modulo = COL % numtasks;
	offset = 0;
	msg_type = FROM_MASTER;
	for (destination = 1; destination < numtasks; destination++) {
	    rows = (destination > modulo) ? average : average + 1;
	    MPI_Send(&COL, 1, MPI_INT, destination, msg_type, MPI_COMM_WORLD);
	    MPI_Send(&rows, 1, MPI_INT, destination, msg_type, MPI_COMM_WORLD);
	    MPI_Send(&offset, 1, MPI_INT, destination, msg_type, 
		    MPI_COMM_WORLD);
	    MPI_Send(&(MAT_PTR + offset), rows * COL, MPI_INT, destination, 
		    msg_type, MPI_COMM_WORLD);
	    offset += rows;
	}

	msg_type = FROM_WORKER;
	for (source = 1; source < numtasks; source++) {
	    MPI_Recv(&rows, 1, MPI_INT, source, msg_type, MPI_COMM_WORLD, 
		    &status);    
	    MPI_Recv(&offset, 1, MPI_INT, source, msg_type, MPI_COMM_WORLD, 
		    &status);
	    MPI_Recv(&(RESULT + offset), rows * COL, MPI_INT, source, 
		    msg_type, MPI_COMM_WORLD, &status);

	}
    } else {
	msg_type = FROM_MASTER;
	MPI_Recv(&COL, 1, MPI_INT, MASTER, msg_type, MPI_COMM_WORLD, &status);
	MPI_Recv(&rows, 1, MPI_INT, MASTER, msg_type, MPI_COMM_WORLD, &status);
	MPI_Recv(&offset, 1, MPI_INT, MASTER, msg_type, MPI_COMM_WORLD,
	       	&status);
	MPI_Recv(&MAT_PTR, rows * COL, MPI_INT, MASTER, msg_type, 
		MPI_COMM_WORLD, &status);

	int i, j, k;
	for (i = 0; i < rows, i++) {
	    for (j = 0; j < COL; j++ ) {
		*(RESULT + i * COL + j) = 0;
		for (k = 0; k < COL; k++) {
		    *(RESULT + i * COL + j) += *(MAT_PTR + i * COL + k) 
			* *(MAT_PTR + k * COL + j);
		}
	    }
	}

	msg_type FROM_WORKER;
	MPI_Send(&rows, 1, MPI_INT, MASTER, msg_type, MPI_COMM_WORLD);
	MPI_Send(&offset, 1, MPI_INT, MASTER, msg_type, MPI_COMM_WORLD);
	MPI_Send(&RESULT, 1, MPI_INT, MASTER, msg_type, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}
