#ifndef _czytelnia_
#define _czytelnia_

/*** Definicje typów zmiennych ***/
typedef struct {
    int l_p; // liczba piszących
    int l_c; // liczba czytających
    int waiting_writers;
    pthread_mutex_t mutex;
    pthread_cond_t czytelnicy_cond;
    pthread_cond_t pisarze_cond;
} cz_t;

/*** Deklaracje procedur interfejsu ***/
void inicjuj(cz_t* czytelnia_p);
void czytam(cz_t* czytelnia_p);
void pisze(cz_t* czytelnia_p);

int my_read_lock_lock(cz_t* czytelnia_p);
int my_read_lock_unlock(cz_t* czytelnia_p);
int my_write_lock_lock(cz_t* czytelnia_p);
int my_write_lock_unlock(cz_t* czytelnia_p);

#endif

