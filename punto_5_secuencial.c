#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

void suma (char *);

int main (int argc, char *argv[]) { 
  pthread_t h1;
  if (argc!= 2) {
    printf ("Indica el nombre de un fichero.\n");
    exit(0);
  }
  pthread_create (&h1, NULL, suma, argv[1]);
  pthread_join (h1, NULL);
  return 0;
}

void suma (char *nombre) {
  char c;
  printf("Thread id = %d\n", pthread_self()); 
  int total = 0;
  int fd, charInNumber;
  fd= open(nombre,O_RDONLY);
  while (read(fd,&c,sizeof(c)!=0)) {
    total =  total + (c -'0');
    printf("%d\n", total);
  }
  close(fd);
}