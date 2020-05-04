#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

// void suma (char *);
void mapearValores (char *, numeros_array *numeros);

typedef struct numeros
{
  int valor;
  int evaluado;
} numeros_array;

int main (int argc, char *argv[]) { 
  // pthread_t h1, h2;

  numeros_array numeros [1000];
  if (argc!= 2) {
    printf ("Indica el nombre de un fichero.\n");
    exit(0);
  }
  mapearValores(argv[1],numeros);
  // pthread_create (&h1, NULL, suma, argv[1]);
  // pthread_join (h1, NULL);
  return 0;
}

// void suma (char *nombre) {
  // char c;
  // printf("Thread id = %d\n", pthread_self()); 
  // int total = 0;
  // int fd, charInNumber;
  // fd= open(nombre,O_RDONLY);
  // while (read(fd,&c,sizeof(c)!=0)) {
  //   total =  total + (c -'0');
  //   printf("%d\n", total);
  // }
  // close(fd);
// }
void mapearValores (char *nombre, numeros_array *numeros){
  int fd, charInNumber;
  char c;
  fd= open(nombre,O_RDONLY);
  while (read(fd,&c,sizeof(c)!=0)) {
    charInNumber =  (c -'0');
    printf("%d\n", charInNumber);
  }
  close(fd);
}