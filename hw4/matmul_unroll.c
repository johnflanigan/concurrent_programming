#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

static double timer() {
    
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double) (tp.tv_sec) + 1e-6 * tp.tv_usec);

}

void transpose(double *B, int n) {
    double *temp;
    temp = (double *) malloc(n * n * sizeof(double));

    /* Transpose B and store in temp */
    int i, j;
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            temp[j*n+i] = B[i*n + j];
        }
    }

    /* Copy from temp to B */
    memcpy(B, temp, sizeof(B));

    free(temp);
}

int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "%s <n>\n", argv[0]);
        fprintf(stderr, "<n>: matrix dimension (nxn dense matrices are created)\n");
        exit(1);
    }

    int n;

    n = atoi(argv[1]);
    assert(n > 0);
    assert(n < 10000);

    fprintf(stderr, "n: %d\n", n);
    fprintf(stderr, "Requires %3.6lf MB memory\n", ((3*8.0*n)*n/1e6));

    double *A, *B, *C;
    
    A = (double *) malloc(n * n * sizeof(double));
    assert(A != 0);
    B = (double *) malloc(n * n * sizeof(double));
    assert(B != 0);
    C = (double *) malloc(n * n * sizeof(double));
    assert(C != 0);


    /* linearized matrices in row-major storage */
    /* A[i][j] would be A[i*n+j] */

    int i, j;

    /* static initalization, so that we can verify output */
    /* using very simple initialization right now */
    /* this isn't a good check for parallel debugging */
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            A[i*n+j] = 1;
            B[i*n+j] = 1;
            C[i*n+j] = 0;
        }
    }

    double elt = 0.0;
    elt = timer();

    transpose(B, n);

    int k;
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            double c_ij0 = 0, c_ij1 = 0, c_ij2 = 0, c_ij3 = 0, c_ij4 = 0, c_ij5 = 0, c_ij6 = 0, c_ij7 = 0;
            for (k=0; k<n; k+=8) {
                c_ij0 += A[i*n+k]*B[j*n+k];
                c_ij1 += A[i*n+(k+1)]*B[j*n+(k+1)];
                c_ij2 += A[i*n+(k+2)]*B[j*n+(k+2)];
                c_ij3 += A[i*n+(k+3)]*B[j*n+(k+3)];
                c_ij4 += A[i*n+(k+4)]*B[j*n+(k+4)];
                c_ij5 += A[i*n+(k+5)]*B[j*n+(k+5)];
                c_ij6 += A[i*n+(k+6)]*B[j*n+(k+6)];
                c_ij7 += A[i*n+(k+7)]*B[j*n+(k+7)];
            }
            for (; k<n; k++) {
                c_ij0 += A[i*n+k]*B[k*n+j];
            }
            C[i*n+j] = c_ij0 + c_ij1 + c_ij2 + c_ij3 + c_ij4 + c_ij5 + c_ij6 + c_ij7;
        }
    }

    elt = timer() - elt;

    /* Verify */
    int verify_failed = 0;
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++) {
            if (C[i*n+j] != n)
                verify_failed = 1;
        }
    }

    if (verify_failed) {
        fprintf(stderr, "ERROR: verification failed, exiting!\n");
        exit(2);
    }

    fprintf(stderr, "Time taken: %3.3lf s.\n", elt);
    fprintf(stderr, "Performance: %3.3lf GFlop/s\n", (2.0*n*n*n)/(elt*1e9));

    /* free memory */
    free(A); free(B); free(C);

    return 0;
}
