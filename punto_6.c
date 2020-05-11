#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <math.h>

struct ArgumentosDeConsola {
  char *nombreFichero;
  int numHilos;
};

struct ArgumentosDeSumaCuadrados {
  void *conjuntoDeNumeros;
  int limSuperior;
  float media;
  double resultado;
};

struct InsumosDeSuma {
   void *conjuntoDeNumeros;
   int limSuperior;
   int resultado;
};

typedef struct ArgumentosDeConsola ArgumentosDeConsola;
typedef struct ArgumentosDeSumaCuadrados ArgumentosDeSumaCuadrados;
typedef struct InsumosDeSuma InsumosDeSuma;

void *contarCaracteres(void *);
void *sumaCaracteres(void *);
void *calculate_standard_deviation(void *);

int main(int argc, char *argv[]) { 
  pthread_t hiloContador;
  if (argc != 3) {
    printf("Debes indicar el nombre de un fichero .txt y seguido el número de hilos a usar\n");
    exit(0);
  }
  ArgumentosDeConsola customArgs;
  ArgumentosDeConsola *customArgsPointer;
  customArgs.nombreFichero = (char *) argv[1];
  customArgs.numHilos = atoi(argv[2]);
  customArgsPointer = &customArgs;
  pthread_create(&hiloContador, NULL, contarCaracteres, (void *) customArgsPointer);
  pthread_join(hiloContador, NULL);
  return 0;
}

void *contarCaracteres(void *args) {
  char c;
  ArgumentosDeConsola *argumentos = (ArgumentosDeConsola *) args;
  char *nombre = argumentos->nombreFichero;
  int numHilos = argumentos->numHilos;
  char *caracteres = (char *) malloc(sizeof(char));
  caracteres[0] = '\0';
  int hiloId = (int) pthread_self();
  printf("[Contador \t (%d)] Iniciando\n", hiloId); 
  int total = 0;
  int fd = open(nombre, O_RDONLY);
  while(read(fd, &c, sizeof(c) != 0)) {
    total += 1;
    if(total != 1){
        caracteres = (char *) realloc(caracteres, total);
    }
    caracteres[total - 1] = c;
  }
  close(fd);
  pthread_t hilo[numHilos];
  InsumosDeSuma insumo[numHilos];
  InsumosDeSuma *punteroAInsumo[numHilos];
  for(int i = 0; i < numHilos; i++){
    insumo[i].conjuntoDeNumeros = &(caracteres[(int) i * (total/numHilos)]);
    insumo[i].limSuperior = (int) (total/numHilos);
    punteroAInsumo[i] = &insumo[i];
    pthread_create(&hilo[i], NULL, (void *) sumaCaracteres, (void *) punteroAInsumo[i]);
  }
  int resultadoFinal = 0;
  for(int i = 0; i < numHilos; i++) {
    pthread_join(hilo[i], NULL);
    printf ("[Contador \t (%d)] Fin del hilo: %d\n", hiloId, i);
  }
  for(int i = 0; i < numHilos; i++) {
    resultadoFinal += insumo[i].resultado;
  }
  float mediaDeNotas = (float) resultadoFinal / (float) total;
  printf("[Contador \t (%d)] Resultado de la suma: %d\n", hiloId, resultadoFinal);
  printf("[Contador \t (%d)] Resultado de la media: %.1f\n", hiloId, mediaDeNotas);
  float notasMenosMedia[total];
  for(int i = 0; i < total; i++){
    notasMenosMedia[i] = ((float) (caracteres[i] - '0')) - mediaDeNotas;
  }
  ArgumentosDeSumaCuadrados cuadraArgs;
  cuadraArgs.conjuntoDeNumeros = notasMenosMedia;
  cuadraArgs.limSuperior = total;
  ArgumentosDeSumaCuadrados *punteroACuadraArgs;
  punteroACuadraArgs = &cuadraArgs;
  pthread_t hiloSumaCuadrados;
  pthread_create(&hiloSumaCuadrados, NULL, (void *) calculate_standard_deviation, (void *) punteroACuadraArgs);
  pthread_join(hiloSumaCuadrados, NULL);
  printf("[Contador \t (%d)] Resultado de la desviación estándar: %.2f\n", hiloId, cuadraArgs.resultado);
  free(caracteres);
  return NULL;
}

void *sumaCaracteres(void *insumo) {
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

void *calculate_standard_deviation(void *args) {
  ArgumentosDeSumaCuadrados *argLocal = (ArgumentosDeSumaCuadrados *) args;
  int hiloId = (int) pthread_self();
  printf("[SumaCuadrada \t (%d)] Iniciando\n", hiloId); 
  float acumulador = 0;
  float *conjunto = (float *) argLocal->conjuntoDeNumeros;
  printf("[SumaCuadrada \t (%d)] Elementos a sumar: %d\n", hiloId, argLocal->limSuperior);
  for(int i = 0; i < argLocal->limSuperior; i++) {
    printf("[SumaCuadrada \t (%d)] Número en curso: %.1f\n", hiloId, conjunto[i]);
    acumulador += conjunto[i] * conjunto[i];
  }
  printf("[SumaCuadrada \t (%d)] Acumulado de cuadrados papeeh %.1f\n", hiloId, acumulador);
  float resultadoParcialSinRaiz = acumulador / ((float) argLocal->limSuperior - 1);
  double resultadoFinalConRaiz = sqrt((double) resultadoParcialSinRaiz);
  argLocal->resultado = resultadoFinalConRaiz;
  return NULL;
}