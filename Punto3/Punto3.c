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
  pthread_t h1;
  
  if (argc!= 2) {
    printf ("Indica el nombre de un fichero.\n");
    exit(0);
  }

  pthread_create(&h1, NULL, cuenta, argv[1]);      
  pthread_join (h1, NULL);
  return 0;
}

