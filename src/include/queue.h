#ifndef QUEUE_H
#define QUEUE_H 1

#include <stddef.h>

typedef struct QueueInt *QueueIntPtr;

// Fixed size queue because only used in graph. Vertices visited only once.
QueueIntPtr QueueInt_new(size_t size);

void QueueInt_delete(QueueIntPtr queue);

void QueueInt_enqueue(QueueIntPtr queue, int e);

int QueueInt_dequeue(QueueIntPtr queue);

int QueueInt_peek(QueueIntPtr queue);

int QueueInt_is_empty(QueueIntPtr queue);

#endif /* QUEUE_H */