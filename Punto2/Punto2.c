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
    pthread_t h[argc-1];
    int count;
    if(argc < 2)
    {
        printf("Debe ingresar al menos un numero para calcular el fatorial\n");
        exit(0);
    }

    for (count=0; count < argc-1 ; count++)
    {
         if(0 != pthread_create (&h[count], NULL, factorial, (void *) argv[count+1]))
            return -1;   
    }
    
     for (count=0; count < argc-1 ; count++)
    {
         pthread_join (h[count], NULL);
         printf ("Final de la ejecucion del hilo %d\n", count);
    }   
    printf ("Final de la ejecucion\n");
    return 0;
}
