#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<time.h>
#include <pthread.h>


void *factorial(void *numero) {
    long long int resultado= 1;
    int num, n= atoi((char *) numero);
    for (num= 2; num<= n; num++) {
      resultado= resultado*num;
      printf ("Factorial de %d, resultado parcial %lld\n", n, resultado);
      sleep (random()%3);
     }
    printf ("El factorial del numero %d es %lld\n", n, resultado);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {  
    srand(time(0)); 
    pthread_t h1, h2;
    if(argc > 3)
    {
        printf("Ingrese solo 2 valores por argumento para calcular el factorial\n");
        exit(0);
    }
    if(0 != pthread_create (&h1, NULL, factorial, (void *) argv[1]))
      return -1;

    if(0 !=  pthread_create (&h2, NULL, factorial, (void *) argv[2]))
      return -1;
  

    pthread_join (h1, NULL);
    pthread_join (h2, NULL);
    return 0;
}

