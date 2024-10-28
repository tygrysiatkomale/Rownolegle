#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3

void *watek_klient(void *arg);

int liczba_wolnych_kufli;
pthread_mutex_t mutex_kufle = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
    pthread_t *tab_klient;
    int *tab_klient_id;

    int l_kl, l_kf, i;

    printf("\nLiczba klientow: ");
    scanf("%d", &l_kl);

    printf("\nLiczba kufli: ");
    scanf("%d", &l_kf);

    liczba_wolnych_kufli = l_kf;

    tab_klient = (pthread_t *)malloc(l_kl * sizeof(pthread_t));
    tab_klient_id = (int *)malloc(l_kl * sizeof(int));
    for (i = 0; i < l_kl; i++) tab_klient_id[i] = i;

    printf("\nOtwieramy pub (busy waiting)!\n");
    printf("\nLiczba wolnych kufli %d\n", l_kf);

    for (i = 0; i < l_kl; i++) {
        pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
    }
    for (i = 0; i < l_kl; i++) {
        pthread_join(tab_klient[i], NULL);
    }

    printf("\nZamykamy pub!\n");

    free(tab_klient);
    free(tab_klient_id);
    pthread_mutex_destroy(&mutex_kufle);
}

void *watek_klient(void *arg_wsk) {
    int moj_id = *((int *)arg_wsk);
    int i;

    printf("\nKlient %d, wchodzę do pubu\n", moj_id);

    for (i = 0; i < ILE_MUSZE_WYPIC; i++) {
        int success = 0;

        do {
            pthread_mutex_lock(&mutex_kufle);
            if (liczba_wolnych_kufli > 0) {
                liczba_wolnych_kufli--;
                printf("\nKlient %d, pobieram kufel (pozostało %d wolnych kufli)\n", moj_id, liczba_wolnych_kufli);
                success = 1;
            }
            pthread_mutex_unlock(&mutex_kufle);

            if (!success) {
                printf("\nKlient %d, czekam na wolny kufel\n", moj_id);
                usleep(10000); // Krótkie czekanie, gdy brak wolnych kufli
            }
        } while (!success);

        // Nalewanie i picie
        printf("\nKlient %d, nalewam z kranu\n", moj_id);
        usleep(30000); // Nalewanie piwa
        printf("\nKlient %d, pije\n", moj_id);
        nanosleep((struct timespec[]){{0, 50000000L}}, NULL); // Picie piwa

        // Odkładanie kufla
        pthread_mutex_lock(&mutex_kufle);
        liczba_wolnych_kufli++;
        printf("\nKlient %d, odkladam kufel (wolnych kufli %d)\n", moj_id, liczba_wolnych_kufli);
        pthread_mutex_unlock(&mutex_kufle);
    }

    printf("\nKlient %d, wychodzę z pubu\n", moj_id);
    return NULL;
}
