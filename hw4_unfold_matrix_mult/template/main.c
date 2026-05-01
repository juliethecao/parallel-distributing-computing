#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"

void
print_help(char *prog_name)
{
    fprintf(stderr, "Multiply two square matrices of size n.\n\n"
            "USAGE:\n"
            "  %s <n> <t> <verb>\n\n", prog_name);
    fprintf(stderr, "ARGUMENTS:\n"
            "  n    : Size of the square matrix. 2 <= n <= 4096\n"
            "  t    : Number of threads used by the multi-thread multiplier.\n"
            "  verb : 0=prints only execution time.\n"
            "         1=print result matrix.\n"
            "         2=print matrix factors and result.\n");
}

int main(int argc, char **argv){
    //help message
    if(argc != 4){
        print_help(argv[0]);
        exit(1);
    }

    //get the command line arguments
    unsigned int n, t, verb;
    n=atoi(argv[1]);
    t=atoi(argv[2]);
    verb=atoi(argv[3]);
    if(n < 2 || 4096 < n){
        fprintf(stderr, "The size of the matrix 'n' (%d) must be >= 2 and <= 4096.\n", n);
        exit(1);
    }
    if(t < 1){
        fprintf(stderr, "The number of threads 't' (%d) must be >= 1.\n", t);
        exit(1);
    }

    //create and fill 3 matrices
    Mat mats[3];
    mat_alloc(&mats[0], n, n);
    mat_alloc(&mats[1], n, n);
    mat_alloc(&mats[2], n, n);
    mat_fill_sequential(&mats[0]);
    mat_fill_diagonal(&mats[1], 2.0); // example

    //print original matrix
    if(verb >= 2){
        printf("A:\n");
        mat_print(&mats[0]);
        printf("B:\n");
        mat_print(&mats[1]);
	
    }

    //run and measure time taken
    double end, start = stopwatch();
    mat_multiply(&mats[0], &mats[1], &mats[2], t);
    end = stopwatch();

    //print results
    if(verb >= 1){
        printf("\nC = A x B:\n");
        mat_print(&mats[2]);
    }

    //print execution time
    printf("%.8f\n", end - start);
    
    return 0;
}
