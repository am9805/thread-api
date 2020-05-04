#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

#define MAXLON 1000

void cuenta (char *);

int main (int argc, char *argv[]) { 
  pthread_t threads[argc];
  if (argc < 2) {
    printf ("Indica al menos un nombre de un fichero.\n");
    exit(0);
  }
  for (int i = 1; i < argc; i++){
    printf ("indice %d, arg %s\n", i, argv[i]);
    pthread_create (&threads[i], NULL, cuenta, argv[i]);
  }
  for (int i = 1; i < argc; i++){
     pthread_join (threads[i], NULL);
     printf ("Fin del hilo: %d\n", i);
  }
  return 0;
}

void cuenta (char *nombre) {
  printf("Thread id = %d\n", pthread_self()); 
  int pos, cont= 0, leidos;
  char cadena[MAXLON];
  int fd;
  fd= open(nombre,O_RDONLY);
  while ((leidos =read(fd,cadena,MAXLON))!= 0) {
    for (pos= 0; pos< leidos; pos++) {
      if ((cadena[pos]== 'a') || (cadena[pos]== 'A')) {
        cont++;
      }
    }
  }
  printf("Fichero %s: %d caracteres 'a' o 'A' encontrados\n", nombre, cont);
  close(fd);
}