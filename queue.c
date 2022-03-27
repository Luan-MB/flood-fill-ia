// GRR20190363 Luan Machado Bernardt

#include "queue.h"
#include <stdio.h>

//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila
int queue_size (queue_t *queue) {

    if (!queue) // Se a fila estiver vazia
        return 0;
    
    queue_t *aux = queue;
    int size = 1;

    while ((aux = aux->next) != queue) // Se a fila possuir ao menos um elemento
        size++;

    return size;
}

//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila
// Retorno: 0 se sucesso, <0 se ocorreu algum erro

int queue_append (queue_t **queue, queue_t *elem) {
    
    if (!(*queue)) { // Se a fila estiver vazia
        *queue = elem;
        (*queue)->next = (*queue)->prev = *queue;
        return 0;
    }
        
    queue_t *aux = *queue;

    while(aux->next != *queue) // Se a fila possuir ao menos um elemento
        aux = aux->next;

    (*queue)->prev = elem;
    elem->prev = aux;
    elem->next = *queue;
    aux->next = elem;

    return 0;
}

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: 0 se sucesso, <0 se ocorreu algum erro
int queue_remove (queue_t **queue, queue_t *elem) {


    if (elem == *queue) { // Caso de elem ser o primeiro elemento da lista
        
        if (*queue == (*queue)->next) { // Caso de elem ser o unico elemento da lista
            *queue = NULL;
        }
        else { // Caso de elem nao ser o unico elemento da lista
            (*queue)->prev->next = (*queue)->next;
            (*queue)->next->prev = (*queue)->prev;
            *queue = (*queue)->next;
        }
        elem->next = elem->prev = NULL;
        return 0;
    }

    queue_t *aux = *queue;

    while ((aux = aux->next) != *queue) { // Percorre a fila em busca de elem
        if (aux == elem) { // Se elem foi encontrado
            aux->prev->next = aux->next;
            aux->next->prev = aux->prev;
            elem->next = elem->prev = NULL;
            return 0;
        }
    }

    perror("### O elemento nao esta na fila ###");
    return 1;
}