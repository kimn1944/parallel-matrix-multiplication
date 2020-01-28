#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

void io(int argc, char **argv);
void multi_thread(int thread_num);
void result_to_file(int num, int duration);
void print_mat(int *mat_ptr, int row, int col);
void mat_square(int *mat_ptr, int row, int col, int *result);
void debug();
void* mult_runner(void *arg);

int *MAT_PTR = NULL;
int *RESULT = NULL;
int ROW = 0;
int COL = 0;
int bug = 0;

struct mult_runner_struct {
    int start;
    int end;
};





int main(int argc, char **argv) {
    // dealing with input
    io(argc, argv);

    // actual work 
    int start = time(NULL);
    multi_thread(atoi(argv[1])); 
    int end = time(NULL);

    // record the result
    result_to_file(atoi(argv[1]), end - start);
    return 0;
}





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
    for (int i = 0; i < ROW; i++) {
	for (int j = 0; j < COL; j++) {
	    fscanf(fptr, "%d", MAT_PTR + i * COL + j);
	} 
    }
    fclose(fptr);
}

void multi_thread(int thread_num) {
    int index = 0;
    int step = ROW / thread_num;
    struct mult_runner_struct threads [thread_num];
    pthread_t tids[thread_num];
    for (int i = 0; i < thread_num; i++) {
	threads[i].start = index;
	threads[i].end = index + step;
	index += step;
	if (i == thread_num - 1) {
	    threads[i].end = ROW;
	}
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&tids[i], &attr, mult_runner, &threads[i]);  
    }
    for (int i = 0; i < thread_num; i++) {
	pthread_join(tids[i], NULL);
    }	
}

void result_to_file(int num, int duration) {
    FILE *fptr = fopen("result.txt", "w");
    if (fptr == NULL) {
	printf("Cannot open the file result.txt\n");
	exit(0);
    }
    fprintf(fptr, "Multithreading with %d threads took: %d seconds\n", num, duration);
    fprintf(fptr, "Resulting matrix:\n");
    fprintf(fptr, "%dx%d\n", ROW, COL);
    for (int i = 0; i < ROW; i++) {
	for (int j = 0; j < COL; j++) {
	    fprintf(fptr, "%d ", *(RESULT + i * COL + j));
	}
	fprintf(fptr, "\n");
    }
    fclose(fptr);
}

void print_mat(int *mat_ptr, int row, int col) { 
    for (int i = 0; i < row; i++) {
	for (int j = 0; j < col; j++) {
	    printf("%d ", *(mat_ptr + i * col + j));
	}
	printf("\n");
    }
}

void mat_square(int *mat_ptr, int row, int col, int *result) {
    for (int i = 0; i < row; i++) {
	for (int j = 0; j < col; j++) {
	    *(result + i * col + j) = 0;
	    for (int k = 0; k < row; k++) {
		*(result + i * col + j) += *(mat_ptr + i * col + k) * *(mat_ptr + k * col + j);
	    }
	}
    }	
}

void debug() {
    printf("%d\n", bug);
    bug++;
}

void* mult_runner(void *arg) {
    struct mult_runner_struct *struct_ptr = (struct mult_runner_struct*) arg;

    for (int i = struct_ptr->start; i < struct_ptr->end; i++) {
	for (int j = 0; j < COL; j++) {
	    *(RESULT + i * COL + j) = 0;
	    for (int k = 0; k < COL; k++) {
		*(RESULT + i * COL + j) += *(MAT_PTR + i * COL + k)
		    * *(MAT_PTR + k * COL + j); 
	    } 
	}
    }

    pthread_exit(0);
}

