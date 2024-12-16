#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>


typedef struct {
    int number;
    double value;
    char name[20];
} MyData;

char recv_buf[100];


int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int prev = (rank - 1 + size) % size;
    int next = (rank + 1) % size;

    MyData data;
    MPI_Status status;

    if (rank == 0) {
        data.number = 42;
        data.value = 3.14;
        strcpy(data.name, "Jakub");

        printf("\nProces %d (inicjalizacja): number=%d, value=%.2f, name=%s\n", rank, data.number, data.value, data.name);

        // Obliczanie rozmiaru bufora
        int buf_size = sizeof(int) + sizeof(double) + strlen(data.name) + 1;
        char *buffer = malloc(buf_size);
        int position = 0;

        MPI_Pack(&data.number, 1, MPI_INT, buffer, buf_size, &position, MPI_COMM_WORLD);
        MPI_Pack(&data.value, 1, MPI_DOUBLE, buffer, buf_size, &position, MPI_COMM_WORLD);
        MPI_Pack(data.name, strlen(data.name)+1, MPI_CHAR, buffer, buf_size, &position, MPI_COMM_WORLD);

        printf("Proces %d wysyla dane do procesu %d\n\n", rank, next);
        MPI_Send(buffer, position, MPI_PACKED, next, 0, MPI_COMM_WORLD);

        free(buffer);

        // jesli zamkniety pierscien
        MPI_Recv(recv_buf, 100, MPI_PACKED, prev, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        MyData receivedData;
        int recv_pos = 0;
        MPI_Unpack(recv_buf, 100, &recv_pos, &receivedData.number, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(recv_buf, 100, &recv_pos, &receivedData.value, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(recv_buf, 100, &recv_pos, receivedData.name, 20, MPI_CHAR, MPI_COMM_WORLD);

        printf("Proces %d otrzymal dane od procesu %d, dane: number=%d, value=%.2f, name=%s\n", rank, prev, receivedData.number, receivedData.value, receivedData.name);
    } else {
        MPI_Recv(recv_buf, 100, MPI_PACKED, prev, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        MyData receivedData;
        int recv_pos = 0;
        MPI_Unpack(recv_buf, 100, &recv_pos, &receivedData.number, 1, MPI_INT, MPI_COMM_WORLD);
        MPI_Unpack(recv_buf, 100, &recv_pos, &receivedData.value, 1, MPI_DOUBLE, MPI_COMM_WORLD);
        MPI_Unpack(recv_buf, 100, &recv_pos, receivedData.name, 20, MPI_CHAR, MPI_COMM_WORLD);

        printf("Proces %d otrzymal dane od procesu %d, dane: number=%d, value=%.2f, name=%s\n", rank, prev, receivedData.number, receivedData.value, receivedData.name);

        receivedData.number += 1;
        receivedData.value += 0.5;
        int len = strlen(receivedData.name);
        if (len < 19) {
            receivedData.name[len] = 'X';
            receivedData.name[len+1] = '\0';
        }

        printf("Proces %d zmodyfikowal dane: number=%d, value=%.2f, name=%s\n", rank, receivedData.number, receivedData.value, receivedData.name);

        int new_buf_size = sizeof(int) + sizeof(double) + strlen(receivedData.name) + 1;
        char *send_buffer = malloc(new_buf_size);
        int send_pos = 0;
        MPI_Pack(&receivedData.number, 1, MPI_INT, send_buffer, new_buf_size, &send_pos, MPI_COMM_WORLD);
        MPI_Pack(&receivedData.value, 1, MPI_DOUBLE, send_buffer, new_buf_size, &send_pos, MPI_COMM_WORLD);
        MPI_Pack(receivedData.name, strlen(receivedData.name)+1, MPI_CHAR, send_buffer, new_buf_size, &send_pos, MPI_COMM_WORLD);


        if (rank != size - 1) {
            printf("Proces %d wysyla dane do procesu %d\n\n", rank, next);
            MPI_Send(send_buffer, send_pos, MPI_PACKED, next, 0, MPI_COMM_WORLD);
        } else {
            MPI_Send(send_buffer, send_pos, MPI_PACKED, 0, 0, MPI_COMM_WORLD);
        }
        free(send_buffer);
    }
    MPI_Finalize();
    return 0;
}