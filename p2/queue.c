#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>

// circular array
typedef struct _queue {
    int size;
    int used;
    int first;
    void **data;
    pthread_mutex_t *m;
    pthread_cond_t *cons;
    pthread_cond_t *prod;
} _queue;

#include "queue.h"

queue q_create(int size) {
    queue q = malloc(sizeof(_queue));

    q->size  = size;
    q->used  = 0;
    q->first = 0;
    q->data  = malloc(size*sizeof(void *));
    for(int i=0; i<size;i++) q->data[i]=NULL;
    q->m=malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(q->m,NULL);
    q->cons=malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->cons,NULL);
    q->prod=malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->prod,NULL);


    return q;
}

int q_elements(queue q) {
    
    return q->used;

}

int q_insert(queue q, void *elem) {

    pthread_mutex_lock(q->m);
    while(q->used==q->size)
      pthread_cond_wait(q->prod,q->m);
    q->data[(q->first+q->used) % q->size] = elem;
    q->used++;
    printf("Insertado, %d/%d\n",q->used,q->size);
    if(q->used==1) pthread_cond_broadcast(q->cons);
    pthread_mutex_unlock(q->m);

    return 1;
}

void *q_remove(queue q) {
    void *res;

    pthread_mutex_lock(q->m);
    while(q->used==0)
      pthread_cond_wait(q->cons,q->m);
    res = q->data[q->first];
    printf("Expulsado, %d/%d\n",q->used,q->size);
    q->first = (q->first+1) % q->size;
    q->used--;

    if(q->used==q->size-1) pthread_cond_broadcast(q->prod);
    pthread_mutex_unlock(q->m);

    return res;
}

void q_destroy(queue q) {
    for (int i=0;i<q->used;i++)printf("%d",*(int *)q->data[(q->first+i) % q->size] );
    pthread_mutex_destroy(q->m);
    free(q->m);
    pthread_cond_destroy(q->cons);
    free(q->cons);
    pthread_cond_destroy(q->prod);
    free(q->prod);
    free(q->data);
    free(q);
}
