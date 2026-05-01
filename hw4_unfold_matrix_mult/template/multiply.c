#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "util.h"

typedef struct {
    Mat *A;
    Mat *B;
    Mat *C;
    unsigned int row_begin; // inclusive
    unsigned int row_end;   // exclusive
} WorkerArgs;

// checks that all matrices are non-null, initialized, and dimensionally compatible
static void validate_inputs(Mat *A, Mat *B, Mat *C)
{
    // check for null matrix pointers
    if(A == NULL || B == NULL || C == NULL){
        fprintf(stderr, "mat_multiply: One or more matrices are NULL.\n");
        exit(1);
    }

    // check that matrix data buffers are allocated
    if(A->ptr == NULL || B->ptr == NULL || C->ptr == NULL){
        fprintf(stderr, "mat_multiply: one or more matrices are not initialized.\n");
        exit(1);
    }

    // A is (m x n), B is (n x p), C is (m x p)
    if(A->n != B->m || A->m != C->m || B->n != C->n){
        fprintf(stderr, "mat_multiply: matrices sizes mismatch.\n");
        exit(1);
    }
}

// assigns each worker a contiguous, non-overlapping stripe of rows from A
static void assign_row_ranges(WorkerArgs *worker_args, Mat *A, Mat *B, Mat *C,
                              unsigned int worker_count)
{
    // divide rows with first extra_rows workers get one additional row
    unsigned int base_rows = A->m / worker_count;
    unsigned int extra_rows = A->m % worker_count;
    unsigned int row_begin = 0;

    // set matrix pointers and row range for each worker
    for(unsigned int worker = 0; worker < worker_count; worker++){
        unsigned int row_count = base_rows + (worker < extra_rows ? 1U : 0U);
        worker_args[worker].A = A;
        worker_args[worker].B = B;
        worker_args[worker].C = C;
        worker_args[worker].row_begin = row_begin;
        worker_args[worker].row_end = row_begin + row_count;
        row_begin += row_count;
    }
}

// thread entry point: computes assigned rows of C using (i, j, k) loop order
static void *worker_multiply(void *arg)
{
    WorkerArgs *args = (WorkerArgs *) arg;
    Mat *A = args->A;
    Mat *B = args->B;
    Mat *C = args->C;

    // each thread handles its assigned row range
    for(unsigned int i = args->row_begin; i < args->row_end; i++){
        // for each output column j, compute dot product of row i of A and column j of B
        for(unsigned int j = 0; j < B->n; j++){
            double sum = 0.0;
            for(unsigned int k = 0; k < A->n; k++){
                sum += A->ptr[i * A->n + k] * B->ptr[k * B->n + j];
            }
            C->ptr[i * C->n + j] = sum;
        }
    }

    return NULL;
}

// parallelizes C = A * B by unfolding the outermost i-loop across threads
void mat_multiply(Mat *A, Mat *B, Mat *C, unsigned int threads) {
    validate_inputs(A, B, C);

    // ensure at least one thread is used
    if(threads < 1){
        threads = 1;
    }

    // never spawn more threads than there are rows
    unsigned int worker_count = threads;
    if(worker_count > A->m)
        worker_count = A->m;

    pthread_t *worker_ids = calloc(worker_count, sizeof(pthread_t));
    WorkerArgs *worker_args = calloc(worker_count, sizeof(WorkerArgs));

    // check that thread and argument arrays were allocated
    if(worker_ids == NULL || worker_args == NULL){
        fprintf(stderr, "mat_multiply: Couldn't allocate worker resources.\n");
        exit(1);
    }

    assign_row_ranges(worker_args, A, B, C, worker_count);

    // spawn all worker threads
    for(unsigned int worker = 0; worker < worker_count; worker++){
        int rc = pthread_create(&worker_ids[worker], NULL, worker_multiply, &worker_args[worker]);
        if(rc != 0){
            fprintf(stderr, "mat_multiply: pthread_create failed.\n");
            exit(1);
        }
    }

    // wait for all threads to finish before returning completed matrix C
    for(unsigned int worker = 0; worker < worker_count; worker++){
        int rc = pthread_join(worker_ids[worker], NULL);
        if(rc != 0){
            fprintf(stderr, "mat_multiply: pthread_join failed.\n");
            exit(1);
        }
    }

    free(worker_ids);
    free(worker_args);
}
