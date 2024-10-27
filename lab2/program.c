#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>
#include "../pomiar_czasu/pomiar_czasu.h"


int zmienna_globalna = 0; // Zmienna globalna, wspólna dla wszystkich wątków

#define ROZMIAR_STOSU 1024*64
#define LICZBA_ITERACJI 100000

// Funkcja wykonywana przez każdy wątek
int funkcja_watku(void* argument) {
    int* zmienna_lokalna = (int*) argument; // Zmienna lokalna przekazana jako argument

    for (int i = 0; i < LICZBA_ITERACJI; i++) {
        zmienna_globalna++;         // Modyfikacja zmiennej globalnej
        (*zmienna_lokalna)++;       // Modyfikacja zmiennej lokalnej
    }

    printf("Wątek zakończył. Zmienna globalna: %d, Zmienna lokalna: %d\n", zmienna_globalna, *zmienna_lokalna);
    return 0;
}

int main() {
    void *stos1, *stos2;
    pid_t pid1, pid2;
    int zmienna_lokalna1 = 0; // Wspólna zmienna lokalna dla pierwszego wątku
    int zmienna_lokalna2 = 0; // Wspólna zmienna lokalna dla drugiego wątku

    stos1 = malloc(ROZMIAR_STOSU); // Alokacja pamięci na stos dla pierwszego wątku
    stos2 = malloc(ROZMIAR_STOSU); // Alokacja pamięci na stos dla drugiego wątku

    if (stos1 == 0 || stos2 == 0) {
        printf("Błąd alokacji stosu\n");
        exit(1);
    }

    inicjuj_czas();

    // Tworzenie pierwszego wątku (procesu)
    pid1 = clone(&funkcja_watku, (void *)stos1 + ROZMIAR_STOSU, 
                 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &zmienna_lokalna1);
    if (pid1 == -1) {
        perror("clone");
        exit(1);
    }

    // Tworzenie drugiego wątku (procesu)
    pid2 = clone(&funkcja_watku, (void *)stos2 + ROZMIAR_STOSU, 
                 CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, &zmienna_lokalna1);
    if (pid2 == -1) {
        perror("clone");
        exit(1);
    }

    // Oczekiwanie na zakończenie obu wątków
    waitpid(pid1, NULL, __WCLONE);
    waitpid(pid2, NULL, __WCLONE);

    // Wyświetlanie wyników po zakończeniu pracy obu wątków
    printf("Main zakończył. Zmienna globalna: %d\n", zmienna_globalna);
    printf("Main zakończył. Zmienna lokalna 1: %d\n", zmienna_lokalna1);
    printf("Main zakończył. Zmienna lokalna 2: %d\n", zmienna_lokalna1);

    free(stos1); // Zwolnienie pamięci dla stosu pierwszego wątku
    free(stos2); // Zwolnienie pamięci dla stosu drugiego wątku

    drukuj_czas();

    return 0;
}
