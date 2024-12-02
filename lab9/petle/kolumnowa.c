#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define WYMIAR 10

int main ()
{
    double a[WYMIAR][WYMIAR];

    // Inicjalizacja tablicy
    for (int i = 0; i < WYMIAR; i++) {
        for (int j = 0; j < WYMIAR; j++) {
            a[i][j] = 1.02 * i + 1.01 * j;
        }
    }

    // Pętla sekwencyjna
    double suma = 0.0;
    for (int i = 0; i < WYMIAR; i++) {
        for (int j = 0; j < WYMIAR; j++) {
            suma += a[i][j];
        }
    }
    printf("Suma wyrazów tablicy (sekwencyjnie): %lf\n", suma);

    // Równoległa pętla wewnętrzna
    double suma_parallel = 0.0;
    for (int i = 0; i < WYMIAR; i++) {
#pragma omp parallel for default(none) shared(a, i) reduction(+:suma_parallel) schedule(dynamic, 2) ordered
        for (int j = 0; j < WYMIAR; j++) {
            int id_w = omp_get_thread_num();
            suma_parallel += a[i][j];
#pragma omp ordered
            printf("(%1d,%1d) -> W_%1d\n", i, j, id_w);
        }
    }

    printf("Suma wyrazów tablicy równolegle (dekompozycja kolumnowa): %lf\n", suma_parallel);

    return 0;
}
