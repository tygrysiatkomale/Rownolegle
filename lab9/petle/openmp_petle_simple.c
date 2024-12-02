#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

#define WYMIAR 18

int main ()
{
    double a[WYMIAR];

    // Inicjalizacja tablicy równolegle
#pragma omp parallel for default(none) shared(a)
    for (int i = 0; i < WYMIAR; i++) {
        a[i] = 1.02 * i;
    }

    // Pętla sekwencyjna
    double suma = 0.0;
    for (int i = 0; i < WYMIAR; i++) {
        suma += a[i];
    }
    printf("Suma wyrazów tablicy (sekwencyjnie): %lf\n", suma);


    // Równoległa pętla z klauzulą reduction i ordered
    double suma_parallel = 0.0;
#pragma omp parallel for default(none) shared(a) reduction(+:suma_parallel) ordered schedule(static)
    for (int i = 0; i < WYMIAR; i++) {
        int id_w = omp_get_thread_num();
        suma_parallel += a[i];
#pragma omp ordered
        printf("a[%2d] -> W_%1d\n", i, id_w);
    }

    printf("\nSuma wyrazów tablicy równolegle: %lf\n", suma_parallel);

    return 0;
}
