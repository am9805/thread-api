#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<time.h>
#include <pthread.h>

void *factorial (void *argumento);

int main (int argc, char *argv[]) {
  srand(time(0));
  pthread_t h1, h2; 
  if (argc!= 3) {
    printf ("Debes indicar dos numeros enteros.\n");
    exit(0);
  }
  pthread_create (&h1, NULL, factorial, (void *) argv[1]);
  pthread_create (&h2, NULL, factorial, (void *) argv[2]);
  pthread_join (h1, NULL);
  pthread_join (h2, NULL);
  return 0;
}

void *factorial (void *argumento) {
  long long int resultado= 1;
  int num, n= atoi((char *) argumento);
  for (num= 2; num<= n; num++) {
    resultado= resultado*num;
    printf ("Factorial de %d, resultado parcial %lld\n", n, resultado);
    sleep (random()%3);
  }
  printf ("El factorial de %d es %lld\n", n, resultado);
  pthread_exit(NULL);
}