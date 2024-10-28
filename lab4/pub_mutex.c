#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

#define ILE_MUSZE_WYPIC 3

void *watek_klient(void *arg);

// Zmienna globalna liczby wolnych kufli
int liczba_wolnych_kufli;
int poczatkowa_liczba_kufli;  // Zmienna do przechowywania początkowej liczby kufli
pthread_mutex_t mutex_kufle = PTHREAD_MUTEX_INITIALIZER; // Mutex dla kufli

int main(void) {
    pthread_t *tab_klient;
    int *tab_klient_id;

    int l_kl, l_kf, i;

    printf("\nLiczba klientow: ");
    scanf("%d", &l_kl);

    printf("\nLiczba kufli: ");
    scanf("%d", &l_kf);

    liczba_wolnych_kufli = l_kf; // Inicjalizacja liczby kufli
    poczatkowa_liczba_kufli = l_kf; // Zapisanie początkowej liczby kufli

    tab_klient = (pthread_t *)malloc(l_kl * sizeof(pthread_t));
    tab_klient_id = (int *)malloc(l_kl * sizeof(int));
    for (i = 0; i < l_kl; i++) tab_klient_id[i] = i;

    printf("\nOtwieramy pub (simple)!\n");
    printf("\nLiczba wolnych kufli %d\n", l_kf);

    for (i = 0; i < l_kl; i++) {
        pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
    }
    for (i = 0; i < l_kl; i++) {
        pthread_join(tab_klient[i], NULL);
    }

    printf("\nZamykamy pub!\n");

    // Sprawdzenie, czy liczba kufli na końcu jest taka sama jak na początku
    if (liczba_wolnych_kufli == poczatkowa_liczba_kufli) {
        printf("\nLiczba kufli na końcu jest zgodna z początkową: %d\n", liczba_wolnych_kufli);
    } else {
        printf("\nBłąd: liczba kufli na końcu (%d) różni się od początkowej (%d)\n", liczba_wolnych_kufli, poczatkowa_liczba_kufli);
    }

    free(tab_klient);
    free(tab_klient_id);
    pthread_mutex_destroy(&mutex_kufle); // Zwalnianie zasobów mutexu
    return 0;
}

void *watek_klient(void *arg_wsk) {
    int moj_id = *((int *)arg_wsk);
    int i;

    printf("\nKlient %d, wchodzę do pubu\n", moj_id);

    for (i = 0; i < ILE_MUSZE_WYPIC; i++) {
        pthread_mutex_lock(&mutex_kufle); // Zablokuj dostęp do kufli
        if (liczba_wolnych_kufli > 0) {
            liczba_wolnych_kufli--;
            printf("\nKlient %d, pobieram kufel (pozostało %d wolnych kufli)\n", moj_id, liczba_wolnych_kufli);
            pthread_mutex_unlock(&mutex_kufle); // Zwolnij dostęp po pobraniu

            // Nalewanie i picie
            printf("\nKlient %d, nalewam z kranu\n", moj_id);
            usleep(30000); // Nalewanie piwa
            printf("\nKlient %d, pije\n", moj_id);
            nanosleep((struct timespec[]){{0, 50000000L}}, NULL); // Picie piwa

            // Odkładanie kufla
            pthread_mutex_lock(&mutex_kufle); // Zablokuj dostęp do kufli
            liczba_wolnych_kufli++;
            printf("\nKlient %d, odkladam kufel (wolnych kufli %d)\n", moj_id, liczba_wolnych_kufli);
            pthread_mutex_unlock(&mutex_kufle); // Zwolnij dostęp po odłożeniu
        } else {
            printf("\nKlient %d, nie ma wolnych kufli, czekam\n", moj_id);
            pthread_mutex_unlock(&mutex_kufle); // Zwolnij dostęp, jeśli brak kufli
            usleep(10000); // Krótkie czekanie, jeśli brak kufli
        }
    }

    printf("\nKlient %d, wychodzę z pubu\n", moj_id);
    return NULL;
}
