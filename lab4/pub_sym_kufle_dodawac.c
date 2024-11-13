#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3

void *watek_klient(void *arg);

int l_kf;              // liczba kufli
int dostepne_kufle;    // aktualna liczba dostępnych kufli

int main(void) {
    pthread_t *tab_klient;
    int *tab_klient_id;
    int l_kl, i;

    printf("\nLiczba klientow: ");
    scanf("%d", &l_kl);
    printf("\nLiczba kufli: ");
    scanf("%d", &l_kf);

    dostepne_kufle = l_kf;

    tab_klient = (pthread_t *)malloc(l_kl * sizeof(pthread_t));
    tab_klient_id = (int *)malloc(l_kl * sizeof(int));
    for (i = 0; i < l_kl; i++) tab_klient_id[i] = i;

    printf("\nOtwieramy pub (unsafe)!\n");

    for (i = 0; i < l_kl; i++) {
        pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
    }
    for (i = 0; i < l_kl; i++) {
        pthread_join(tab_klient[i], NULL);
    }

    printf("\nZamykamy pub! Dostępne kufle na koniec: %d\n", dostepne_kufle);

    free(tab_klient);
    free(tab_klient_id);

    return 0;
}

void *watek_klient(void *arg_wsk) {
    int moj_id = *((int *)arg_wsk);
    int i;

    printf("\nKlient %d wchodzi do pubu\n", moj_id);

    for (i = 0; i < ILE_MUSZE_WYPIC; i++) {
        usleep(1000);
        if (dostepne_kufle > 0) {
            dostepne_kufle--;
            printf("\nKlient %d pobiera kufel. Kufli pozostało: %d\n", moj_id, dostepne_kufle);
        } else {
            printf("\nKlient %d nie może pobrać kufla - brak dostępnych\n", moj_id);
        }

        printf("\nKlient %d nalewa piwo\n", moj_id);
        usleep(50000);
        printf("\nKlient %d pije piwo\n", moj_id);
        usleep(50000);

        usleep(1000);
        dostepne_kufle++; 
        printf("\nKlient %d oddaje kufel. Kufli dostępnych: %d\n", moj_id, dostepne_kufle);
    }

    printf("\nKlient %d wychodzi z pubu\n", moj_id);
    return NULL;
}
