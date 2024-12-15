#include<stdlib.h>
#include<stdio.h>
#include<omp.h>

int f_threadprivate;
#pragma omp threadprivate(f_threadprivate)

int main() {
    omp_set_num_threads(5);

    #pragma omp parallel default(none)
    {
        f_threadprivate = omp_get_thread_num();
        #pragma omp critical
        printf("Pierwszy obszar: watek %d, f_threadprivate = %d\n", omp_get_thread_num(), f_threadprivate);
    }

    #pragma omp parallel default(none)
    {
        #pragma omp critical
        printf("Drugi obszar: watek %d, f_threadprivate = %d\n", omp_get_thread_num(), f_threadprivate);
    }

    return 0;
}
