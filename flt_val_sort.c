#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#endif
#include "qsort.h"

typedef int bool;
#define true 1
#define false 0

static double timer() {
    
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double) (tp.tv_sec) + 1e-6 * tp.tv_usec);

    /* The code below is for another high resolution timer */
    /* I'm using gettimeofday because it's more portable */

    /*
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return ((double) (tp.tv_sec) + 1e-9 * tp.tv_nsec);
    */
}

/* comparison routine for C's qsort */
static int qs_cmpf(const void *u, const void *v) {

    if (*(float *)u > *(float *)v)
        return 1;
    else if (*(float *)u < *(float *)v)
        return -1;
    else
        return 0;
}

/* inline QSORT() comparison routine */
#define inline_qs_cmpf(a,b) ((*a)<(*b))


static int inline_qsort_serial(const float *A, const int n, const int num_iterations) {

    fprintf(stderr, "N %d\n", n);
    fprintf(stderr, "Using inline qsort implementation\n");
    fprintf(stderr, "Execution times (ms) for %d iterations:\n", num_iterations);

    int iter;
    double avg_elt;

    float *B;
    B = (float *) malloc(n * sizeof(float));
    assert(B != NULL);

    avg_elt = 0.0;

    for (iter = 0; iter < num_iterations; iter++) {
        
        int i;

        for (i=0; i<n; i++) {
            B[i] = A[i];
        }

        double elt;
        elt = timer();

        QSORT(float, B, n, inline_qs_cmpf);

        elt = timer() - elt;
        avg_elt += elt;
        fprintf(stderr, "%9.3lf\n", elt*1e3);

        /* correctness check */
        for (i=1; i<n; i++) {
            assert(B[i] >= B[i-1]);
        }

    }

    avg_elt = avg_elt/num_iterations;
    
    free(B);

    fprintf(stderr, "Average time: %9.3lf ms.\n", avg_elt*1e3);
    fprintf(stderr, "Average sort rate: %6.3lf MB/s\n", 4.0*n/(avg_elt*1e6));
    return 0;

}

static int qsort_serial(const float *A, const int n, const int num_iterations) {

    fprintf(stderr, "N %d\n", n);
    fprintf(stderr, "Using C qsort\n");
    fprintf(stderr, "Execution times (ms) for %d iterations:\n", num_iterations);

    int iter;
    double avg_elt;

    float *B;
    B = (float *) malloc(n * sizeof(float));
    assert(B != NULL);

    avg_elt = 0.0;

    for (iter = 0; iter < num_iterations; iter++) {
        
        int i;

        for (i=0; i<n; i++) {
            B[i] = A[i];
        }

        double elt;
        elt = timer();

        qsort(B, n, sizeof(float), qs_cmpf);

        elt = timer() - elt;
        avg_elt += elt;
        fprintf(stderr, "%9.3lf\n", elt*1e3);

        /* correctness check */
        for (i=1; i<n; i++) {
            assert(B[i] >= B[i-1]);
        }

    }

    avg_elt = avg_elt/num_iterations;
    
    free(B);

    fprintf(stderr, "Average time: %9.3lf ms.\n", avg_elt*1e3);
    fprintf(stderr, "Average sort rate: %6.3lf MB/s\n", 4.0*n/(avg_elt*1e6));
    return 0;

}

static int binary_search( float value, const float* a, int left, int right )
{
    long low  = left;
    long high;
    if (left > right + 1) {
        high = left;
    } else {
        high = right + 1;
    }
    while( low < high )
    {
        long mid = ( low + high ) / 2;
        if ( value <= a[ mid ] ) high = mid;
        else                        low  = mid + 1; // because we compared to a[mid] and the value was larger than a[mid].
                                                    // Thus, the next array element to the right from mid is the next possible
                                                    // candidate for low, and a[mid] can not possibly be that candidate.
    }
    return high;
}

static void exchange( int* A, int* B )
{
    int t = *A;
    *A = *B;
    *B = t;
}

static void merge_ptr( const float* a_start, const float* a_end, const float* b_start, const float* b_end, float* dst )
{
    while( a_start < a_end && b_start < b_end ) {
        if ( *a_start <= *b_start )  *dst++ = *a_start++;    // if elements are equal, then a[] element is output
        else                        *dst++ = *b_start++;
    }
    while( a_start < a_end ) *dst++ = *a_start++;
    while( b_start < b_end ) *dst++ = *b_start++;
}

static void merge_parallel(const float* t, int p1, int r1, int p2, int r2, float* a, int p3 )
{
    int length1 = r1 - p1 + 1;
    int length2 = r2 - p2 + 1;
    if ( length1 < length2 ) {
        exchange(      &p1,      &p2 );
        exchange(      &r1,      &r2 );
        exchange( &length1, &length2 );
    }
    if ( length1 == 0 ) return;
    if (( length1 + length2 ) <= 16 )
        merge_ptr( &t[ p1 ], &t[ p1 + length1 ], &t[ p2 ], &t[ p2 + length2 ], &a[ p3 ] );
    else {
        int q1 = ( p1 + r1 ) / 2;
        int q2 = binary_search( t[ q1 ], t, p2, r2 );
        int q3 = p3 + ( q1 - p1 ) + ( q2 - p2 );
        a[ q3 ] = t[ q1 ];
        #pragma omp parallel sections
        {
            #pragma omp section
            { merge_parallel( t, p1,     q1 - 1, p2, q2 - 1, a, p3     ); }
            #pragma omp section
            { merge_parallel( t, q1 + 1, r1,     q2, r2,     a, q3 + 1 ); }
        }
    }
}

static void merge_sort_split(const float* src, int l, int r, float* dst, bool srcToDst) {

	if (r == l) {    // termination/base case of sorting a single element
        if (srcToDst)  dst[ l ] = src[ l ];    // copy the single element from src to dst
        return;
    }
    int m = ( r + l ) / 2;

    #pragma omp parallel sections
    {
        #pragma omp section
    	{ merge_sort_split( src, l,     m, dst, !srcToDst ); } // reverse direction of srcToDst for the next level of recursion
        #pragma omp section 
        { merge_sort_split( src, m + 1, r, dst, !srcToDst ); } // reverse direction of srcToDst for the next level of recursion
    }
    if ( srcToDst ) merge_parallel_L5( src, l, m, m + 1, r, dst, l );
    else            merge_parallel_L5( dst, l, m, m + 1, r, src, l );
}

static int merge_sort_parallel(const float *A, const int n, const int num_iterations) {

    fprintf(stderr, "N %d\n", n);
    fprintf(stderr, "Using parallel merge sort\n");
    fprintf(stderr, "Execution times (ms) for %d iterations:\n", num_iterations);

    int iter;
    double avg_elt;

    float *B;
    B = (float *) malloc(n * sizeof(float));
    assert(B != NULL);

    /* Create C because merge sort is not in place */
    float *C;
    C = (float *) malloc(n * sizeof(float));
    assert(C != NULL);

    avg_elt = 0.0;

    for (iter = 0; iter < num_iterations; iter++) {
        
        int i;

        for (i=0; i<n; i++) {
            B[i] = A[i];
        }

        double elt;
        elt = timer();

        /* Sorting of B occurs here */
        merge_sort_split(B, 0, n-1, C, true);

        elt = timer() - elt;
        avg_elt += elt;
        fprintf(stderr, "%9.3lf\n", elt*1e3);

        /* correctness check */
        for (i=1; i<n; i++) {
            assert(C[i] >= C[i-1]);
        }

    }

    avg_elt = avg_elt/num_iterations;
    
    free(B);
    free(C);

    fprintf(stderr, "Average time: %9.3lf ms.\n", avg_elt*1e3);
    fprintf(stderr, "Average sort rate: %6.3lf MB/s\n", 4.0*n/(avg_elt*1e6));
    return 0;

}

/* generate different inputs for testing sort */
int gen_input(float *A, int n, int input_type) {

    int i;

    /* uniform random values */
    if (input_type == 0) {

        srand(123);
        for (i=0; i<n; i++) {
            A[i] = ((float) rand())/((float) RAND_MAX);
        }

    /* sorted values */    
    } else if (input_type == 1) {

        for (i=0; i<n; i++) {
            A[i] = (float) i;
        }

    /* almost sorted */    
    } else if (input_type == 2) {

        for (i=0; i<n; i++) {
            A[i] = (float) i;
        }

        /* do a few shuffles */
        int num_shuffles = (n/100) + 1;
        srand(1234);
        for (i=0; i<num_shuffles; i++) {
            int j = (rand() % n);
            int k = (rand() % n);

            /* swap A[j] and A[k] */
            float tmpval = A[j];
            A[j] = A[k];
            A[k] = tmpval;
        }

    /* array with single unique value */    
    } else if (input_type == 3) {

        for (i=0; i<n; i++) {
            A[i] = 1.0;
        }

    /* sorted in reverse */    
    } else {

        for (i=0; i<n; i++) {
            A[i] = (float) (n + 1.0 - i);
        }

    }

    return 0;

}


int main(int argc, char **argv) {

    if (argc != 4) {
        fprintf(stderr, "%s <n> <input_type> <alg_type>\n", argv[0]);
        fprintf(stderr, "input_type 0: uniform random\n");
        fprintf(stderr, "           1: already sorted\n");
        fprintf(stderr, "           2: almost sorted\n");
        fprintf(stderr, "           3: single unique value\n");
        fprintf(stderr, "           4: sorted in reverse\n");
        fprintf(stderr, "alg_type 0: use C qsort\n");
        fprintf(stderr, "         1: use inline qsort\n");
        fprintf(stderr, "         2: use parallel merge sort\n");
        exit(1);
    }

    int n;

    n = atoi(argv[1]);

    assert(n > 0);
    assert(n <= 1000000000);

    float *A;
    A = (float *) malloc(n * sizeof(float));
    assert(A != 0);

    int input_type = atoi(argv[2]);
    assert(input_type >= 0);
    assert(input_type <= 4);

    gen_input(A, n, input_type);

    int alg_type = atoi(argv[3]);

    int num_iterations = 10;
    
    assert((alg_type == 0) || (alg_type == 1) || (alg_type == 2));

    if (alg_type == 0) {
        qsort_serial(A, n, num_iterations);
    } else if (alg_type == 1) {    
        inline_qsort_serial(A, n, num_iterations);
    } else if (alg_type == 2) {
    	merge_sort_parallel(A, n, num_iterations);
    }

    free(A);

    return 0;
}
