#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Funkcja dla wątku
void *zadanie_watku(void *arg) {
    int identyfikator = *((int *)arg);  // Przypisanie przesłanego identyfikatora
    printf("Wątek: ID systemowy: %lu, Identyfikator wątku: %d\n", pthread_self(), identyfikator);
    return NULL;
}

int main() {
    int liczba_watkow = 5;
    pthread_t watki[liczba_watkow];
    int identyfikatory[liczba_watkow];

    // Tworzenie wątków
    for (int i = 0; i < liczba_watkow; i++) {
        identyfikatory[i] = i;
        if (pthread_create(&watki[i], NULL, zadanie_watku, &identyfikatory[i]) != 0) {
            perror("Błąd przy tworzeniu wątku");
            exit(1);
        }
    }

    // Oczekiwanie na zakończenie wątków
    for (int i = 0; i < liczba_watkow; i++) {
        if (pthread_join(watki[i], NULL) != 0) {
            perror("Błąd przy dołączaniu wątku");
            exit(1);
        }
    }

    return 0;
}
