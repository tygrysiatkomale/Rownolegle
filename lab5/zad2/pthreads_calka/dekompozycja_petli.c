#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include<stdlib.h>

double funkcja ( double x );

double calka_zrownoleglenie_petli(double a, double b, double dx, int l_w);

static int l_w_global=0;

static double calka_global=0.0;
static double a_global;
static double b_global;
static double dx_global;
static int N_global;
pthread_mutex_t mutex_calka;

void* calka_fragment_petli_w(void* arg_wsk);

double calka_zrownoleglenie_petli(double a, double b, double dx, int l_w){

  int N = ceil((b-a)/dx);
  double dx_adjust = (b-a)/N;

  printf("Obliczona liczba trapezów: N = %d, dx_adjust = %lf\n", N, dx_adjust);
  //printf("a %lf, b %lf, n %d, dx %.12lf (dx_adjust %.12lf)\n", a, b, N, dx, dx_adjust);
  pthread_mutex_init(&mutex_calka, NULL);

  // Przekazanie wartości do zmiennych globalnych
    a_global = a;
    b_global = b;
    dx_global = dx_adjust;
    N_global = N;
    l_w_global = l_w;

  // Alokacja pamięci
    pthread_t *threads = malloc(l_w * sizeof(pthread_t));
    int *thread_ids = malloc(l_w * sizeof(int));

  
  // tworzenie struktur danych do obsługi wielowątkowości


  // Tworzenie wątków
    for (int i = 0; i < l_w; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, calka_fragment_petli_w, (void *)&thread_ids[i]);
    }


  // Oczekiwanie na zakończenie pracy wątków i zebranie wyników
    for (int i = 0; i < l_w; i++) {
        pthread_join(threads[i], NULL);
    }

    // Zwolnienie pamięci
    pthread_mutex_destroy(&mutex_calka);
    free(threads);
    free(thread_ids);


  return(calka_global);
}

void* calka_fragment_petli_w(void* arg_wsk){

  int my_id = *(int*)arg_wsk;

  // double a, b, dx; // skąd pobrać dane a, b, dx, N, l_w ? 
  // int N, l_w;      // wariant 1 - globalne

  // a = a_global; // itd. itp. - wartości globalne nadaje calka_zrownoleglenie_petli

  // dekompozycja cykliczna
  int my_start = my_id;
  int my_end = N_global;
  int my_stride = l_w_global;

  // dekompozycja blokowa
  // int my_start = my_id * (N_global / l_w_global);
  // int my_end = (my_id + 1) * (N_global / l_w_global);
  // int my_stride = 1;

  // something else ? (dekompozycja blokowo-cykliczna)

  // printf("\nWątek %d: my_start %d, my_end %d, my_stride %d\n", 
	//  my_id, my_start, my_end, my_stride);

  if (my_id == l_w_global - 1){
    my_end = N_global;
  }

  int i;
  double calka = 0.0;
  for(i=my_start; i<my_end; i+=my_stride){

    double x1 = a_global + i*dx_global;
    calka += 0.5*dx_global*(funkcja(x1)+funkcja(x1+dx_global));
    //printf("i %d, x1 %lf, funkcja(x1) %lf, całka = %.15lf\n", 
    //	   i, x1, funkcja(x1), calka);

  }
  pthread_mutex_lock(&mutex_calka);
  calka_global += calka;
  pthread_mutex_unlock(&mutex_calka);

  pthread_exit(NULL);

}

