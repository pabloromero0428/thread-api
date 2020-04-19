#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<time.h>

#define MAXLON 1000

void *cuenta (void *nombre) {
  char *nameFichero = (char *)nombre;
  int pos, cont= 0, leidos;
  char cadena[MAXLON];
  int fd;
  fd= open(nameFichero,O_RDONLY);
  while ((leidos =read(fd,cadena,MAXLON))!= 0) {
    for (pos= 0; pos< leidos; pos++) {
      if ((cadena[pos]== 'a') || (cadena[pos]== 'A')) {
        cont++;
      }
    }
  }
  printf("Fichero %s: %d caracteres 'a' o 'A' encontrados\n", nameFichero, cont);
  close(fd);
  pthread_exit(NULL);   
}


int main (int argc, char *argv[]) { 
  pthread_t h[argc-1];
  int count;
  if (argc < 2) {
    printf ("Indica el nombre de un fichero.\n");
    exit(0);
  }
    for (count=0; count < argc-1 ; count++)
    {
         if(0 != pthread_create (&h[count], NULL, cuenta, (void *) argv[count+1]))
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