#include <pthread.h>
#include <stdio.h>

static int threads_to_wait;    // Liczba wątków do oczekiwania
static int threads_waiting;    // Liczba wątków, które dotarły do bariery
static int generation = 0;     // Numer pokolenia bariery
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // Muteks
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;    // Zmienna warunkowa

void bariera_init(int n) {
    threads_to_wait = n;
    threads_waiting = 0;
    generation = 0;
}

void bariera(void) {
    pthread_mutex_lock(&mutex);
    int my_generation = generation;

    threads_waiting++;

    if (threads_waiting == threads_to_wait) {
        threads_waiting = 0;
        generation++;
        pthread_cond_broadcast(&cond);
    } else {
        while (my_generation == generation) {
            pthread_cond_wait(&cond, &mutex);
        }
    }

    pthread_mutex_unlock(&mutex);
}
