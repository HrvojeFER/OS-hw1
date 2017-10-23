#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

unsigned long int zadnji;
int pauza = 0;

void periodicki_ispis (int sig){
  printf ("Zadnji prosti broj je: %ld.\n", zadnji);
}

void postavi_pauzu (int sig){
  pauza = 1 - pauza;
}

void prekini(int sig){
  printf("Zadnji prosti broj je: %ld.\n", zadnji);
  exit(0);
}

int prost (unsigned long n){
  if (!(n & 1)) return 0;
  
  unsigned long i = 3;
  unsigned long max = sqrt (n);
  
  while (i <= max){
    if (!(n % i)) return 0;
    i++;
  }
  
  return 1;
}

int main (){

  sigset (SIGALRM, periodicki_ispis);
  sigset (SIGINT, postavi_pauzu);
  sigset (SIGTERM, prekini);

  unsigned long int broj = 3;
  struct itimerval t;
  
  t.it_value.tv_sec = 5;
  t.it_value.tv_usec = 0;
  t.it_interval.tv_sec = 5;
  t.it_interval.tv_usec = 0;

  setitimer (ITIMER_REAL, &t, NULL);

  while(1){
    if (prost(broj))
      zadnji = broj;
    broj++;
    while(pauza)
      pause();
  }
  
  return 0;
  
}
