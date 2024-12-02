#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "czytelnia.h"

int my_read_lock_lock(cz_t* cz_p) {
    pthread_mutex_lock(&cz_p->mutex);
    printf("czytelnik próbuje wejść: l_c=%d, l_p=%d\n", cz_p->l_c, cz_p->l_p);
    while (cz_p->l_p > 0 || cz_p->waiting_writers) {
        pthread_cond_wait(&cz_p->czytelnicy_cond, &cz_p->mutex);
    }
    cz_p->l_c++;
    printf("czytelnik wszedł: l_c=%d, l_p=%d\n", cz_p->l_c, cz_p->l_p);
    pthread_mutex_unlock(&cz_p->mutex);
    return 0;
}

int my_read_lock_unlock(cz_t* cz_p) {
    pthread_mutex_lock(&cz_p->mutex);
    cz_p->l_c--;
    printf("czytelnik wychodzi: l_c=%d, l_p=%d\n", cz_p->l_c, cz_p->l_p);
    if (cz_p->l_c == 0) {
        pthread_cond_signal(&cz_p->pisarze_cond);
    }
    pthread_mutex_unlock(&cz_p->mutex);
    return 0;
}

int my_write_lock_lock(cz_t* cz_p) {
    pthread_mutex_lock(&cz_p->mutex);
    printf("pisarz próbuje wejść: l_c=%d, l_p=%d\n", cz_p->l_c, cz_p->l_p);
    cz_p->waiting_writers = 1;
    while (cz_p->l_c > 0 || cz_p->l_p > 0) {
        pthread_cond_wait(&cz_p->pisarze_cond, &cz_p->mutex);
    }
    cz_p->waiting_writers = 0;
    cz_p->l_p++;
    printf("pisarz wszedł: l_c=%d, l_p=%d\n", cz_p->l_c, cz_p->l_p);
    pthread_mutex_unlock(&cz_p->mutex);
    return 0;
}

int my_write_lock_unlock(cz_t* cz_p) {
    pthread_mutex_lock(&cz_p->mutex);

    cz_p->l_p--;  // Pisarz wychodzi
    printf("pisarz wychodzi: l_c=%d, l_p=%d\n", cz_p->l_c, cz_p->l_p);

    // Wybudzaj wątki w zależności od sytuacji
    if (cz_p->waiting_writers) {
        pthread_cond_signal(&cz_p->pisarze_cond);  // Priorytet dla pisarzy
    } else {
        pthread_cond_broadcast(&cz_p->czytelnicy_cond);  // Jeśli brak pisarzy, wybudzaj czytelników
    }

    pthread_mutex_unlock(&cz_p->mutex);
    return 0;
}

void inicjuj(cz_t* cz_p) {
    cz_p->l_p = 0;
    cz_p->l_c = 0;
    cz_p->waiting_writers = 0;

    pthread_mutex_init(&cz_p->mutex, NULL);
    pthread_cond_init(&cz_p->czytelnicy_cond, NULL);
    pthread_cond_init(&cz_p->pisarze_cond, NULL);
}

void czytam(cz_t* cz_p) {
    pthread_mutex_lock(&cz_p->mutex);
    printf("\t\t\tczytam:  l_c=%d, l_p=%d\n", cz_p->l_c, cz_p->l_p);
    pthread_mutex_unlock(&cz_p->mutex);
    usleep(rand() % 3000000);
}

void pisze(cz_t* cz_p) {
    pthread_mutex_lock(&cz_p->mutex);
    printf("\t\t\tpisze:   l_c=%d, l_p=%d\n", cz_p->l_c, cz_p->l_p);
    pthread_mutex_unlock(&cz_p->mutex);
    usleep(rand() % 3000000);
}

