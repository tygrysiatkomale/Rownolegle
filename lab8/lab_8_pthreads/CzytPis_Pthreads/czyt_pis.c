#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#include"czytelnia.h"

void *funkcja_czytelnika(void *);
void *funkcja_pisarza(void *);

int main() {
    int i;
    pthread_t pisarze[1], czytelnicy[10];
    cz_t czytelnia;

    inicjuj(&czytelnia);

    for (i = 0; i < 5; i++) {
        pthread_create(&pisarze[i], NULL, funkcja_pisarza, (void *)&czytelnia);
    }
    for (i = 0; i < 10; i++) {
        pthread_create(&czytelnicy[i], NULL, funkcja_czytelnika, (void *)&czytelnia);
    }
    for (i = 0; i < 5; i++) {
        pthread_join(pisarze[i], NULL);
    }
    for (i = 0; i < 10; i++) {
        pthread_join(czytelnicy[i], NULL);
    }
}

void *funkcja_czytelnika(void *arg) {
    cz_t* cz_p = (cz_t *)arg;

    for (;;) {
        usleep(rand() % 1000000);
        printf("czytelnik %lu - przed zamkiem\n", pthread_self());

        my_read_lock_lock(cz_p);

        // Korzystanie z zasobów czytelni
        printf("czytelnik %lu - wchodzę\n", pthread_self());
        czytam(cz_p);
        printf("czytelnik %lu - wychodzę\n", pthread_self());

        my_read_lock_unlock(cz_p);

        printf("czytelnik %lu - po zamku\n", pthread_self());
    }
}

void *funkcja_pisarza(void *arg) {
    cz_t* cz_p = (cz_t *)arg;

    for (;;) {
        usleep(rand() % 3000000);
        printf("pisarz %lu - przed zamkiem\n", pthread_self());

        my_write_lock_lock(cz_p);

        // Korzystanie z zasobów czytelni
        printf("pisarz %lu - wchodzę\n", pthread_self());
        pisze(cz_p);
        printf("pisarz %lu - wychodzę\n", pthread_self());

        my_write_lock_unlock(cz_p);

        printf("pisarz %lu - po zamku\n", pthread_self());
    }
}

