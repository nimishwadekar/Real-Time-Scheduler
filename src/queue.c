#include "queue.h"
#include <stdlib.h>

struct QueueInt
{
	int *array;
	size_t front; // front points to next index to dequeue from.
	size_t back; // back points to next index to enqueue to.
};


QueueIntPtr QueueInt_new(size_t size)
{
	QueueIntPtr queue = malloc(sizeof(struct QueueInt));
	if(!queue)
	{
		return NULL;
	}

	queue->array = malloc(size * sizeof(int));
	if(!queue->array)
	{
		free(queue);
		return NULL;
	}

	queue->front = 0;
	queue->back = 0;

	return queue;
}


void QueueInt_delete(QueueIntPtr queue)
{
	free(queue->array);
	free(queue);
}


void QueueInt_enqueue(QueueIntPtr queue, int e)
{
	queue->array[queue->back] = e;
	queue->back++;
}


int QueueInt_dequeue(QueueIntPtr queue)
{
	int retval = queue->array[queue->front];
	queue->front++;
	return retval;
}


int QueueInt_peek(QueueIntPtr queue)
{
	return queue->array[queue->front];
}


int QueueInt_is_empty(QueueIntPtr queue)
{
	return queue->front == queue->back;
}