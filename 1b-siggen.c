#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#define N 6
#define GP 0

int sig[] = {GP, SIGUSR1, SIGUSR2, SIGILL, SIGCONT, SIGINT};
int pid;

void ubi(int i){
  kill(pid, SIGKILL);
  exit(0);
}

int main (int argc, char*argv[]){
  pid = atoi(argv[1]);
  sigset(SIGINT, ubi);

  srand(time(NULL));
  while(1){
    sleep(rand() % 2 + 3);
    int i = rand() % (N - 2) + 1;
    printf ("%d\n", i);
    kill (pid, sig[i]);
  }

  return 0;
}
  
