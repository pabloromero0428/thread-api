#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<time.h>

#define MAXLON 1000

typedef struct
{
  int *length;
  int *data;
} array;

typedef struct
{
  array arr; 
  int start; 
  int end; 
  int *res;
} argsThread;

void addElement(array arr, int newValue);
void* sequentialSum(void *param);

int main (int argc, char *argv[]) { 
  if (argc!= 2) {
    printf ("Indica el nombre de un fichero.\n");
    exit(0);
  }
  array numbers = {
    (int*)malloc(sizeof(int)),
    (int*)malloc(0*sizeof(int))
  };
  char *nameFile = (char *)argv[1];
  int count, readSize;
  char cadena[MAXLON];
  int fd = open(nameFile,O_RDONLY);
  while ((readSize =read(fd,cadena,MAXLON))!= 0) {
    char *nextNumber = (char*)malloc(0*sizeof(char));
    int len = 0;
    for (count= 0; count< readSize; count++) {
      if(cadena[count] == ','){
        addElement(numbers, atoi(nextNumber));
        nextNumber = NULL;
        nextNumber = (char*)realloc(nextNumber, 0*sizeof(char));
        len = 0;
      }else{
        nextNumber = (char*)realloc(nextNumber, sizeof(nextNumber) + sizeof(char));
        nextNumber[len++] = cadena[count];
      }
    }
    free(nextNumber);
  }
  //Suma secuencial
  clock_t endTime;
  double allTime;
  clock_t startTime = clock();
  int resSequentialSum = 0;
  argsThread arg0 = { numbers, 0, numbers.length[0], &resSequentialSum }; 
  sequentialSum(&arg0);
  printf ("Suma secuencial = %i ", resSequentialSum);
  endTime = clock();
  allTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
  printf("(El tiempo es: %f segundos) \n", allTime);
  //Suma por hilos
  int res1, res2;
  pthread_t thread1, thread2;
  argsThread arg1 = { numbers, 0, (int)numbers.length[0]/2, &res1 };
  argsThread arg2 = { numbers, (int)numbers.length[0]/2, (int)numbers.length[0], &res2 };
  startTime = clock();
  pthread_create(&thread1, NULL, sequentialSum, &arg1);
  pthread_create(&thread2, NULL, sequentialSum, &arg2);   
  pthread_join (thread1, NULL);
  pthread_join (thread2, NULL);
  endTime = clock();
  allTime = (double)(endTime - startTime) / CLOCKS_PER_SEC;
  printf ("Suma con hilos = %i ", res1 + res2);
  printf("(El tiempo es: %f segundos) \n", allTime);


  free(numbers.length);
  free(numbers.data);
  close(fd);
  

  
  return 0;
}

void addElement(array arr, int newValue){
    arr.data = (int*)realloc(arr.data, sizeof(arr.data) + sizeof(int));
    arr.data[*arr.length] = newValue;
    arr.length[0]++;
}

void* sequentialSum(void *param){
  argsThread *args = (argsThread *) param;
  *args->res = 0;
  for (int i = args->start; i < args->end; i++)
  {
    *args->res += args->arr.data[i];
  }
  return (void *) NULL;
}