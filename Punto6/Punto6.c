#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

typedef struct
{
  int *length;
  float *data;
} array;
void addElement(array *arr, float newValue);

typedef struct
{
  array arr; 
  int start; 
  int end; 
  float *res;
  float average;
} argsThread;

void calculate_standard_deviation(double n, double sum, double *res);
void* sequentialSumStandardDeviation(void *param);

int main(int argc, char *argv[])
{
  if (argc!= 3) {
    printf ("Indica el nombre de un fichero y el numero de hilos a utilizar.\n");
    exit(0);
  }
  char *nameFile = (char *)argv[1];
  int nroThreads = atoi(argv[2]);
  FILE* stream = fopen(nameFile, "r");
  array notes = {
    (int*)malloc(sizeof(int)),
    NULL
  };
  char line[10];
  float average = 0;
  while (fgets(line, 10, stream))
  {
      char* tmp = strdup(line);
      float note = strtof(tmp, NULL);
      free(tmp);
      addElement(&notes, note);
      average += note;
  }
  average /= notes.length[0];

  //Creacion de hilos
  if(nroThreads > notes.length[0]){
    nroThreads = notes.length[0];
  }else if(nroThreads == 0){
    nroThreads = 1;
  }
  pthread_t threads[nroThreads];
  argsThread argThreads[nroThreads];
  float resThreads[nroThreads];
  int currentStart = 0;
  
  for (int count=0; count < nroThreads ; count++)
  {
    int end;
    if(count + 1 >= nroThreads){
      end = notes.length[0];
    }else{
      end = currentStart + (notes.length[0]/nroThreads);
    }
    argThreads[count] = (argsThread){notes,currentStart,end,&resThreads[count],average};
    currentStart = end;
    pthread_create(&threads[count], NULL, sequentialSumStandardDeviation, &argThreads[count]);
  }
  double sum = 0, n = (double)notes.length[0], response = 0;
  for (int count=0; count < nroThreads; count++)
  {
    pthread_join(threads[count], NULL);
    sum += (double)resThreads[count];
  }
  calculate_standard_deviation(n, sum, &response);
  printf("La desviación estándar es %f \n", response);
}

void calculate_standard_deviation(double n, double sum, double *res){
  *res = sqrt((1/(n - 1))*sum);
}

void* sequentialSumStandardDeviation(void *param){
  argsThread *args = (argsThread *) param;
  *args->res = 0;
  for (int i = args->start; i < args->end; i++)
  {
    *args->res += (args->arr.data[i] - args->average)*(args->arr.data[i] - args->average);
  }
  return (void *) NULL;
}

void addElement(array *arr, float newValue){
  arr->data = (float*)realloc(arr->data, sizeof(float) * (arr->length[0] + 1));
  arr->data[arr->length[0]] = newValue;
  arr->length[0]++;
}