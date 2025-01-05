#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

#define SCALAR double
#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

int main(int argc, char *argv[]){ 
	MPI_Init(&argc, &argv);
  
	int rank, size;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int N = 0;
	if (rank==0){
		printf("Podaj maksymalna liczbe wyrazow do obliczenia przyblizenia PI\n");
		scanf("%d", &N);
	}

	MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int block_size = N / size;
	int my_start = rank * block_size;
	int my_end = (rank + 1) * block_size;

	if (rank == size - 1) {
		my_end = N;
	}

	SCALAR local_suma_plus = 0.0;
	SCALAR local_suma_minus = 0.0;

	for (int i = my_start; i < my_end; i++) {
		int j = 1 + 4*i;
		local_suma_plus += 1.0 / j;
		local_suma_minus += 1.0 / (j + 2.0);
	}

	SCALAR global_suma_plus = 0.0;
	SCALAR global_suma_minus = 0.0;
	MPI_Reduce(&local_suma_plus, &global_suma_plus, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(&local_suma_minus, &global_suma_minus, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	if(rank == 0) {
        SCALAR pi_approx = 4.0 * (global_suma_plus - global_suma_minus);
		double diff = M_PI - pi_approx; 
        printf("PI obliczone:  %20.15lf\n", pi_approx);
        printf("PI prawidlowe: %20.15lf\n", M_PI);
		printf("Roznica:       %20.15lf\n", diff);
    }


	MPI_Finalize();
	return 0;
}

