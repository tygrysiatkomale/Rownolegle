#include<stdio.h>

#include "mpi.h"

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int prev = (rank - 1 + size) % size;
    int next = (rank + 1) % size;

    int data;
    MPI_Status status;

    if (rank == 0) {
        data = 100;
        printf("Proces %d wysyla liczbe %d do procesu %d\n", rank, data, next);
        MPI_Send(&data, 1, MPI_INT, next, 0, MPI_COMM_WORLD);

        MPI_Recv(&data, 1, MPI_INT, prev, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("Proces %d odebral liczbe %d od procesu %d (zamkniety pierscien)\n", rank, data, prev);
    } else {
        MPI_Recv(&data, 1, MPI_INT, prev, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("Proces %d odebral liczbe %d od procesu %d\n", rank, data, prev);

        data += 1;

        if (rank != size-1) {
            printf("Proces %d wysyla liczbe %d do procesu %d\n", rank, data, next);
            MPI_Send(&data, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        } else {
            MPI_Send(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    
    return 0;
}