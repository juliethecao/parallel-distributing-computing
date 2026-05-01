#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "util.h" //implementing

double
stopwatch(void)
{
    struct timespec ts;
    //timespec_get(&ts, TIME_UTC); //-std=c11
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double) ts.tv_sec + ts.tv_nsec/1000000000.0;
}

void
mat_alloc(Mat *mat, unsigned int m, unsigned int n)
{
    if(m == 0 || n == 0){
        fprintf(stderr, "mat_alloc: Matrix size invalid.\n");
        exit(1);
    }
    mat->ptr = (double*) calloc(m*n, sizeof(double));
    if(mat->ptr == NULL){
        fprintf(stderr, "mat_alloc: Couldn't allocate memory for matrix.\n");
        exit(1);
    }
    mat->m = m;
    mat->n = n;
    return;
}

void
mat_free(Mat *mat)
{
    if(mat == NULL){
        fprintf(stderr, "mat_free: The matrix is NULL.\n");
        exit(1);
    }    
    if(mat->ptr == NULL){
        fprintf(stderr, "mat_free: Matrix has already been freed.\n");
        exit(1);
    }
    free(mat->ptr);
    mat->ptr = NULL;
    mat->m = 0;
    mat->n = 0;
    return;
}

void
mat_fill_rand(Mat *mat)
{
    if(mat == NULL){
        fprintf(stderr, "mat_fill_rand: The matrix is NULL.\n");
        exit(1);
    }
    if(mat->ptr == NULL){
        fprintf(stderr, "mat_fill_rand: The matrix is not initialized.\n");
        exit(1);
    }
    int n;   
    for(int i=0; i < mat->m*mat->n; i++){
        n=rand();
        mat->ptr[i] = ((double)n/RAND_MAX)*100;
    }
    return;
}

void
mat_fill_sequential(Mat *mat)
{
    if(mat == NULL){
        fprintf(stderr, "mat_fill_sequential: The matrix is NULL.\n");
        exit(1);
    }
    if(mat->ptr == NULL){
        fprintf(stderr, "mat_fill_sequential: The matrix is not initialized.\n");
        exit(1);
    }
    for(int i=0; i < mat->m*mat->n; i++){
        mat->ptr[i] = (double)i;
    }
    return;
}

void
mat_fill_diagonal(Mat *mat, double diag)
{
    if(mat == NULL){
        fprintf(stderr, "mat_fill_diagonal: The matrix is NULL.\n");
        exit(1);
    }
    if(mat->ptr == NULL){
        fprintf(stderr, "mat_fill_diagonal: The matrix is not initialized.\n");
        exit(1);
    }
    unsigned int off = 0, max_off= mat->m * mat->n;
    while(off < max_off){
        mat->ptr[off] = diag;
        off += mat->n + 1;
    }
    return;
}

void
mat_copy(Mat *to, Mat *from)
{
    if(from == NULL){
        fprintf(stderr, "mat_copy: matrix 'from' is NULL.\n");
        exit(1);
    }
    if(to == NULL){
        fprintf(stderr, "mat_copy: matrix 'to' is NULL.\n");
        exit(1);
    }
    if(from->ptr == NULL){
        fprintf(stderr, "mat_copy: matrix 'from' is not initialized.\n");
        exit(1);
    }
    if(to->ptr == NULL){
        fprintf(stderr, "mat_copy: matrix 'to' is not initialized.\n");
        exit(1);
    }
    if(from->m != to->m || from->n != to->n){
        fprintf(stderr, "mat_copy: matrices sizes do not match.\n");
        exit(1);
    }
    for(int i=0; i < from->m*from->n; i++){
        to->ptr[i] = from->ptr[i];
    }
    return;
}

void mat_print(Mat *mat){
    if(mat == NULL){
        fprintf(stderr, "mat_print: The matrix is NULL.\n");
        exit(1);
    }
    if(mat->ptr == NULL){
        fprintf(stderr, "mat_print: The matrix is not initialized.\n");
        exit(1);
    }
    unsigned int i, j, m = mat->m, n = mat->n;
    for(i=0; i < m; i++){
        for(j=0; j < n; j++){
            printf("  %5.1f", mat->ptr[i*n + j]);
        }
        printf("\n");
    }
    return;
}
