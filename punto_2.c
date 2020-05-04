#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<time.h>
#include <pthread.h>

void *factorial (void *argumento);
// void *createThreads (void *argumento);

int main (int argc, char *argv[]) {
  srand(time(0));
  pthread_t threads[argc];
  if (argc <= 1) {
    printf ("Debes indicar al menos un entero.\n");
    exit(0);
  }
  for (int i = 1; i < argc; i++){
    printf ("indice %d, arg %s\n", i, argv[i]);
    pthread_create (&threads[i], NULL, factorial,(void *) argv[i]);
  }
  for (int i = 1; i < argc; i++){
     pthread_join (threads[i], NULL);
     printf ("Fin del hilo: %d\n", i);
  }
  return 0;
}

void *factorial (void *argumento) {
  printf("Thread id = %d\n", pthread_self()); 
  long long int resultado= 1;
  int num, n = atoi((char *) argumento);
  for (num= 2; num<= n; num++) {
    resultado= resultado*num;
    printf ("Factorial de %d, resultado parcial %lld\n", n, resultado);
    sleep (random()%3);
  }
  printf ("El factorial de %d es %lld\n", n, resultado);
  pthread_exit(NULL);
}