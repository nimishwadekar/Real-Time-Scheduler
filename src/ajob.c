#include "ajob.h"
#include "vector.h"
#include <stdlib.h>

struct AJobQueue
{
	size_t size, front;
	VectorDoublePtr release_times;
	VectorDoublePtr exec_times;
};


AJobQueuePtr AJobQueue_new(void)
{
	AJobQueuePtr queue = malloc(sizeof(struct AJobQueue));
	if(!queue)
	{
		return NULL;
	}

	queue->size = 0;
	queue->front = 0;
	queue->release_times = VectorDouble_new(); // check null
	queue->exec_times = VectorDouble_new(); // check null
	return queue;
}


void AJobQueue_delete(AJobQueuePtr queue)
{
	VectorDouble_delete(queue->exec_times);
	VectorDouble_delete(queue->release_times);
	free(queue);
}


void AJobQueue_enqueue(AJobQueuePtr queue, double release_time, double exec_time)
{
	VectorDouble_append(queue->release_times, release_time);
	VectorDouble_append(queue->exec_times, exec_time);
	queue->size++;
}


void AJobQueue_dequeue(AJobQueuePtr queue)
{
	queue->front++;
}


int AJobQueue_peek(AJobQueuePtr queue, double *release_time, double *exec_time)
{
	*release_time = VectorDouble_get(queue->release_times, queue->front);
	*exec_time = VectorDouble_get(queue->exec_times, queue->front);

	return queue->front;
}


int AJobQueue_is_empty(AJobQueuePtr queue)
{
	return queue->front == queue->size;
}


void AJobQueue_update_exec_time(AJobQueuePtr queue, double remaining_time)
{
	VectorDouble_set(queue->exec_times, queue->front, remaining_time);
}