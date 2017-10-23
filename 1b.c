#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#define N 6
#define VRIJEME_PREKIDA 5
#define GP 0

int OZNAKA_CEKANJA[N];
int PRIORITET[N];
int TEKUCI_PRIORITET = 0;

int sig[] = {GP, SIGUSR1, SIGUSR2, SIGILL, SIGCONT, SIGINT};

void ispis(char type, int n, char c, int j){
  for (int i = 0 ; i < j ; ++i)
    printf ("  -");
  if (type == 'i') printf("%3d", n);
  else printf ("%3c", c);
  for (int i = j + 1 ; i < N ; ++i)
    printf ("  -");
  
  printf("  O_CEK[");
  for (int i = 0 ; i < N ; ++i)
    printf ("%3d", OZNAKA_CEKANJA[i]);
  printf("] TEK_PRIOR=%d PRIOR[", TEKUCI_PRIORITET);
  for (int i = 0 ; i < N ; ++i)
    printf ("%3d", PRIORITET[i]);
  printf("]\n");

  return;
}

void zabrani_prekidanje(){
  for (int i = 1 ; i < N ; ++i)
    sighold(sig[i]);

  return;
}

void dozvoli_prekidanje(){
  for (int i = 1 ; i < N ; ++i)
    sigrelse(sig[i]);

  return;
}

void obrada_prekida(int i){
  ispis('c', 0, 'P', i);
  for (int j = 1 ; j <= VRIJEME_PREKIDA ; ++j){
    sleep(1);
    ispis('i', j, 0, i);
  }
  ispis('c', 0, 'K', i);

  return;
}

void obrada_signala(int i){
  zabrani_prekidanje();
  int n = -1;
  int x;
  for (int j = 1 ; j < N ; ++j)
    if (i == sig[j]) n = j;
  
  ispis('c', 0, 'X', n);
  OZNAKA_CEKANJA[n]++;
  
  do {
    x = 0;
    for (int j = TEKUCI_PRIORITET + 1 ; j < N ; ++j)
      if (OZNAKA_CEKANJA[j])
	x = j;
    if (x){
      OZNAKA_CEKANJA[x]--;
      PRIORITET[x] = TEKUCI_PRIORITET;
      TEKUCI_PRIORITET = x;
      dozvoli_prekidanje();
      obrada_prekida(x);
      zabrani_prekidanje();
      TEKUCI_PRIORITET = PRIORITET[x];
    }
  } while (x > 0);

  dozvoli_prekidanje();
  return;
}

int main (){
  for (int i = 1 ; i < N ; ++i)
    sigset (sig[i], obrada_signala);

  printf("Proces obrade prekida, PID = %ld.\n", getpid());
  printf(" GP S1 S2 S3 S4 S5\n-----------------\n");
  int proteklo_vrijeme = 0;
  while(1){
    sleep(1);
    ++proteklo_vrijeme;
    ispis('i', proteklo_vrijeme, 0, GP);
  }

  printf("Killed.\n");
  return 0;
}
