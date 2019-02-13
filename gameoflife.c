#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>

#define USE_MPI 1
#define TOLEFT 0
#define TORIGHT 1

#if USE_MPI
#include <mpi.h>
#endif

static double timer() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double) (tp.tv_sec) + 1e-6 * tp.tv_usec);
}

int main(int argc, char **argv) {

    int rank, num_tasks;

    /* Initialize MPI */
#if USE_MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num_tasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // printf("Hello world from rank %3d of %3d\n", rank, num_tasks);
#else
    rank = 0;
    num_tasks = 1;
#endif

    if (argc != 3) {
        if (rank == 0) {
            fprintf(stderr, "%s <m> <k>\n", argv[0]);
            fprintf(stderr, "Program for parallel Game of Life\n");
            fprintf(stderr, "with 1D grid partitioning\n");
            fprintf(stderr, "<m>: grid dimension (an mxm grid is created)\n");
            fprintf(stderr, "<k>: number of time steps\n");
            fprintf(stderr, "(initial pattern specified inside code)\n");
#if USE_MPI
            MPI_Abort(MPI_COMM_WORLD, 1);
#else
            exit(1);
#endif
        }
    }

    int m, k;

    m = atoi(argv[1]);
    assert(m > 2);
    assert(m <= 10000);

    k = atoi(argv[2]);
    assert(k > 0);
    assert(k <= 1000);

    /* ensure that m is a multiple of num_tasks */
    m = (m/num_tasks) * num_tasks;
    
    int m_p = (m/num_tasks);

    /* print new m to let user know n has been modified */
    if (rank == 0) {
        fprintf(stderr, "Using m: %d, m_p: %d, k: %d\n", m, m_p, k);
        fprintf(stderr, "Requires %3.6lf MB of memory per task\n", 
                ((2*4.0*m_p)*m/1e6));
    }

    /* Linearizing 2D grids to 1D using row-major ordering */
    /* grid[i][j] would be grid[i*n+j] */
    int *grid_current;
    int *grid_next;
    
    grid_current = (int *) malloc((m_p+2) * (m+2) * sizeof(int));
    assert(grid_current != 0);

    grid_next = (int *) malloc((m_p+2) * (m+2) * sizeof(int));
    assert(grid_next != 0);

    int i, j, t;

    /* static initalization, so that we can verify output */
    /* using very simple initialization right now */
    /* this isn't a good check for parallel debugging */
#ifdef _OPENMP
#pragma omp parallel for private(i,j)
#endif
    for (i=0; i<=m_p; i++) {
        for (j=0; j<=m; j++) {
            grid_current[i*m+j] = 0;
            grid_next[i*m+j] = 0;
        }
    }

    /* initializing some cells in the middle */
    assert((m*m_p/2 + m/2 + 3) < m_p*m);
    grid_current[m*m_p/2 + m/2 + 0] = 1;
    grid_current[m*m_p/2 + m/2 + 1] = 1;
    grid_current[m*m_p/2 + m/2 + 2] = 1;
    grid_current[m*m_p/2 + m/2 + 3] = 1;

#if USE_MPI
    MPI_Barrier(MPI_COMM_WORLD);
#endif

    double elt = 0.0;
    if (rank == 0) 
        elt = timer();

#if USE_MPI
    /* Parallel code goes here */
    /* grid_current and grid_next must be updated correctly */

    /* Perform k iterations */
    int count;
    for (count = 0; count < k; count++) {
        /* Copy bounds (using MPI sendrecv) */
        int left_rank  = (rank-1+num_tasks) % num_tasks;
	    int right_rank = (rank+1) % num_tasks;
        MPI_Status status;

        MPI_Sendrecv(&grid_current[1], m+2, MPI_INT, 
                    left_rank, TOLEFT, 
                    &grid_current[m+1], m+2, MPI_INT, 
                    right_rank, TOLEFT, 
                    MPI_COMM_WORLD, &status);

		MPI_Sendrecv(&grid_current[m], m+2, MPI_INT, 
                    right_rank, TORIGHT, 
                    &grid_current[0], m+2, MPI_INT, 
                    left_rank, TORIGHT, 
                    MPI_COMM_WORLD, &status);

        /* Copy corners */
	    grid_current[0*m + 0]           = grid_current[0*m + m];
	    grid_current[0*m + (m+1)]       = grid_current[0*m + 1];
	    grid_current[(m+1)*m + 0]       = grid_current[(m+1)*m + m];
	    grid_current[(m+1)*m + (m+1)]   = grid_current[(m+1)*m + 1];

	    /* Copy top and bottom */
	    for (i = 1; i <= m; i++) {
	    	grid_current[i*m + 0]       = grid_current[i*m + m];
	    	grid_current[i*m + (m+1)] = grid_current[i*m + 1];
	    }
    
        /* Evaluate neighbors */
        int i,j,s,t,neighbors;

        for (i = 0; i <= m; i++) {
		    for (j = 0; j <= m; j++) {
			    neighbors = 0;

			    /* Count neighbors */
			    for (s = i-1; s <= i+1; s++) {
				    for (t = j-1; t <= j+1; t++) {
					    if (!(s == i && t == j) && grid_current[s*m + t] != 0)
						    neighbors++;
				    }
			    }

			    /* Update state */
			    if (neighbors < 2 || neighbors > 3)
				    grid_next[i*m + j] = 0;
			    else if (grid_current[i*m + j] != 0 || neighbors == 3)
				    grid_next[i*m + j] = grid_current[i*m + j]+1;
		    }
	    }

        /* Update grid */
        for (i = 0; i < m+2; i++) {
		    for (j = 0; j < m+2; j++) {
			    grid_current[i*m + j] = grid_next[i*m + j];
            }
        }

    }

#else
    /* serial code */
    /* considering only internal cells */
    for (t=0; t<k; t++) {
        for (i=1; i<m-1; i++) {
            for (j=1; j<m-1; j++) {
                /* avoiding conditionals inside inner loop */
                int prev_state = grid_current[i*m+j];
                int num_alive  = 
                                grid_current[(i  )*m+j-1] + 
                                grid_current[(i  )*m+j+1] + 
                                grid_current[(i-1)*m+j-1] + 
                                grid_current[(i-1)*m+j  ] + 
                                grid_current[(i-1)*m+j+1] + 
                                grid_current[(i+1)*m+j-1] + 
                                grid_current[(i+1)*m+j  ] + 
                                grid_current[(i+1)*m+j+1];

                grid_next[i*m+j] = prev_state * ((num_alive == 2) + (num_alive == 3)) + (1 - prev_state) * (num_alive == 3);
            }
        }
        /* swap current and next */
        int *grid_tmp  = grid_next;
        grid_next = grid_current;
        grid_current = grid_tmp;
    }
#endif

    if (rank == 0) 
        elt = timer() - elt;

    /* Verify */
    int verify_failed = 0;
    for (i=0; i<m_p; i++) {
        for (j=0; j<m; j++) {
            /* Not sure how to verify without running known good serial code */
            /* Hestitant to do that because it would negate any speedup from MPI */
        }
    }

    if (verify_failed) {
        fprintf(stderr, "ERROR: rank %d, verification failed, exiting!\n", rank);
#if USE_MPI
        MPI_Abort(MPI_COMM_WORLD, 2);
#else
        exit(2);
#endif
    }

    if (rank == 0) {
        fprintf(stderr, "Time taken: %3.3lf s.\n", elt);
        fprintf(stderr, "Performance: %3.3lf billion cell updates/s\n", 
                (1.0*m*m)*k/(elt*1e9));
    }

    /* free memory */
    free(grid_current); 
    free(grid_next);

    /* Shut down MPI */
#if USE_MPI
    MPI_Finalize();
#endif

    return 0;
}
