#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

extern void bariera_init(int);
extern void bariera(void);

#define LICZBA_W 4

pthread_t watki[LICZBA_W];
int use_barrier = 1; // 1 - włączona bariera, 0 - wyłączona

void *cokolwiek(void *arg);

int main(int argc, char *argv[]) {
    if (argc > 1) {
        use_barrier = atoi(argv[1]); // Ustawienie trybu na podstawie argumentu
    }

    int i, indeksy[LICZBA_W];
    for (i = 0; i < LICZBA_W; i++) indeksy[i] = i;

    if (use_barrier) {
        bariera_init(LICZBA_W);
    }

    for (i = 0; i < LICZBA_W; i++) {
        pthread_create(&watki[i], NULL, cokolwiek, (void *)&indeksy[i]);
    }

    for (i = 0; i < LICZBA_W; i++) pthread_join(watki[i], NULL);

    pthread_exit(NULL);
}

void *cokolwiek(void *arg) {
    int moj_id = *((int *)arg);

    printf("przed bariera - watek %d\n", moj_id);

    if (use_barrier) {
        bariera(); // Wywołanie bariery, jeśli jest włączona
    }

    printf("po barierze - watek %d\n", moj_id);

    pthread_exit((void *)0);
}
