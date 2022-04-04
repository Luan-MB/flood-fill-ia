// Luan Machado Bernardt | GRR20190363

#ifndef __QUEUE__
#define __QUEUE__

typedef struct queue_t {
   struct queue_t *prev ; 
   struct queue_t *next ; 
} queue_t ;

int queue_size (queue_t *queue);
void queue_append (queue_t **queue, queue_t *elem);
void queue_remove (queue_t **queue, queue_t *elem) ;

#endif