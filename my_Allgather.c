#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <mpi.h>

#define MPI_ALLGATHER_TAG 0

int my_Allgather(int *sendbuf, int n, int nprocs, int *recvbuf) {
    int i;
    for (i=0; i<n*nprocs; i++) {
        recvbuf[i] = 0;
    }

    int mask, dst_tree_root, my_tree_root, send_offset, recv_offset, comm_size, dst, rank, recvcount, mpi_errno, curr_cnt, position, last_recv_cnt;
    MPI_Comm comm;
    MPI_Status status;
    MPI_Datatype recvtype;
    MPI_Aint recvtype_extent;
    
    comm_size = nprocs;
    mask = 0x1;
    i = 0;
    last_recv_cnt = 0;

    while (mask < comm_size) {
        dst = rank ^ mask;
	               
        dst_tree_root = dst >> i;
        dst_tree_root <<= i;
       
        my_tree_root = rank >> i;
        my_tree_root <<= i;

        send_offset = my_tree_root * recvcount * recvtype_extent;
        recv_offset = dst_tree_root * recvcount * recvtype_extent;
	               
        if (dst < comm_size) {
            MPI_Sendrecv((sendbuf + send_offset), curr_cnt, MPI_INT, 
                        dst, MPI_ALLGATHER_TAG, 
                        (recvbuf + recv_offset), (comm_size-dst_tree_root)*recvcount, MPI_INT, 
                        dst, MPI_ALLGATHER_TAG, 
                        comm, &status);

	                   
            MPI_Get_count(&status, recvtype, &last_recv_cnt);
            curr_cnt += last_recv_cnt;
        }

        mask <<= 1;
        i++;
    }

    return 0;
}

int main(int argc, char **argv) {

    int rank, nprocs;
    int i;

    /* Initialize MPI Environment */
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* change the input size here */
    int n;
    if (argc == 2) {
        n = atoi(argv[1]);
    } else {
        n = 4;
    }
    if (rank == 0) {
        fprintf(stderr, "n: %d\n", n);
    }

    int *sendbuf;
    int *recvbuf1;
    int *recvbuf2;
    
    sendbuf  = (int *) malloc(n*sizeof(int));
    assert(sendbuf != 0);

    for (i=0; i<n; i++) {
        sendbuf[i] = (rank+1);
    }

    recvbuf1 = (int *) malloc(n*nprocs*sizeof(int));
    assert(recvbuf1 != 0);

    recvbuf2 = (int *) malloc(n*nprocs*sizeof(int));
    assert(recvbuf2 != 0);

    MPI_Allgather(sendbuf, n, MPI_INT, recvbuf1, n, MPI_INT, MPI_COMM_WORLD);
    my_Allgather(sendbuf, n, nprocs, recvbuf2);
    
    /* verify that my_Allgather works correctly */
    for (i=0; i<n*nprocs; i++) {
        assert(recvbuf1[i] == recvbuf2[i]);
    }

    free(sendbuf); free(recvbuf1); free(recvbuf2);

    /* Terminate MPI environment */
    MPI_Finalize();

    return 0;
}
