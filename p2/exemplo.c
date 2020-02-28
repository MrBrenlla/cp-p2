#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



void * encher1(void * qu){
  queue q = (queue) qu;

  for (int i = 0; i<25; i++){
    int* j=malloc(sizeof(int));
    *j=i;
    q_insert(q,(void*)j);
  }
}

void* encher2(void * qu){
  queue q = (queue) qu;
  for (int i = -10; i>-35; i--){
    int* j=malloc(sizeof(int));
    *j=i;
    q_insert(q,(void*)j);
  }
}

void *coller(void * qu){
  queue q = (queue) qu;
  void * j;
  for (int i=0; i<25;i++){
    j= q_remove(q) ;
    free(j);
  }
}

void fallo(){
  printf("\n" );
  printf("exemploE1 [qsize] [threads] \n\n\n" );
  printf("  qsize   -> Tamaño maximo da cola, pordefecto 10\n\n" );
  printf("  threads -> creanse x(por defecto = 1) conxuntos de 4 threads, 1 engade[0..24],\n");
  printf("             o segundo [-10..-34] e os dous restantes sustran 25 cada un\n\n\n" );
  exit(0);
}

int main(int argc, char *argv[]){
  int threads=1, qSize=10;

  switch(argc){
    case 1: break;
    case 3: threads=strtol(argv[2],NULL,10);
    case 2: qSize = strtol(argv[1],NULL,10); break;
    default: fallo();
  }


  pthread_t T[threads*4];

  queue q =q_create(qSize);

  for(int i=0; i<threads;i++){
    pthread_create(&T[i*4],NULL,encher1,q);
    pthread_create(&T[i*4+1],NULL,encher2,q);
    pthread_create(&T[i*4+2],NULL,coller,q);
    pthread_create(&T[i*4+3],NULL,coller,q);
  }
  for(int i=0; i<threads*4;i++) pthread_join(T[i], NULL);
  q_destroy(q);
}
