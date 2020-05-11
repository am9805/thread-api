#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>

struct InsumosDeSuma {
   void *conjuntoDeNumeros;
   int limSuperior;
   int resultado;
};

typedef struct InsumosDeSuma InsumosDeSuma;

void *contarCaracteres(void *);
void *suma(void *);

int main(int argc, char *argv[]) { 
  pthread_t hiloContador;
  if (argc!= 2) {
    printf("Debes indicar el nombre de un fichero .txt\n");
    exit(0);
  }
  pthread_create(&hiloContador, NULL, contarCaracteres, argv[1]);
  pthread_join(hiloContador, NULL);
  return 0;
}

void *contarCaracteres(void *nombre) {
  char c;
  pthread_t hiloSumador1, hiloSumador2;
  char *caracteres = (char *) malloc(sizeof(char));
  caracteres[0] = '\0';
  int hiloId = (int) pthread_self();
  printf("[Contador \t (%d)] Iniciando\n", hiloId); 
  int total = 0;
  int fd = open((char *) nombre, O_RDONLY);
  while(read(fd, &c, sizeof(c) != 0)) {
    total += 1;
    if(total != 1){
        caracteres = (char *) realloc(caracteres, total);
    }
    caracteres[total - 1] = c;
  }
  close(fd);
  InsumosDeSuma insumo1, insumo2;
  InsumosDeSuma *punteroAInsumo1;
  InsumosDeSuma *punteroAInsumo2;
  insumo1.conjuntoDeNumeros = caracteres;
  insumo1.limSuperior = (int) (total/2);
  insumo2.conjuntoDeNumeros = &(caracteres[total/2]);
  insumo2.limSuperior = (total % 2 == 0) ? (int) (total/2) : (int) (total/2) + 1;
  punteroAInsumo1 = &insumo1;
  punteroAInsumo2 = &insumo2;
  int resultadoFinal;
  pthread_create(&hiloSumador1, NULL, (void *) suma, (void *) punteroAInsumo1);
  pthread_create(&hiloSumador2, NULL, (void *) suma, (void *) punteroAInsumo2);
  pthread_join(hiloSumador1, NULL);
  pthread_join(hiloSumador2, NULL);
  resultadoFinal = insumo1.resultado + insumo2.resultado;
  printf("[Contador \t (%d)] Resultado de la suma del hilo 1: %d\n", hiloId, insumo1.resultado);
  printf("[Contador \t (%d)] Resultado de la suma del hilo 2: %d\n", hiloId, insumo2.resultado);
  printf("[Contador \t (%d)] Resultado de la suma: %d\n", hiloId, resultadoFinal);
  free(caracteres);
  return NULL;
}

void *suma(void *insumo) {
    InsumosDeSuma *insumoLocal = (InsumosDeSuma *) insumo;
    int hiloId = (int) pthread_self();
    printf("[Sumador \t (%d)] Iniciando\n", hiloId); 
    int acumulador = 0;
    char *conjunto = (char *) insumoLocal->conjuntoDeNumeros;
    printf("[Sumador \t (%d)] Elementos a sumar: %d\n", hiloId, insumoLocal->limSuperior);
    for(int i = 0; i < insumoLocal->limSuperior; i++) {
        printf("[Sumador \t (%d)] Número en curso: %c\n", hiloId, conjunto[i]);
        acumulador += ((char) conjunto[i]) - '0';
    }
    insumoLocal->resultado = acumulador;
    printf("[Sumador \t (%d)] Acumulado papeeh %d\n", hiloId, insumoLocal->resultado);
    return NULL;
}