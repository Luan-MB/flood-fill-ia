#ifndef __QUEUE__
#define __QUEUE__

typedef struct queue_t {
   struct queue_t *prev ;  // aponta para o elemento anterior na fila
   struct queue_t *next ;  // aponta para o elemento seguinte na fila
} queue_t ;

int queue_size (queue_t *queue);
void queue_append (queue_t **queue, queue_t *elem);
void queue_remove (queue_t **queue, queue_t *elem) ;

#endif