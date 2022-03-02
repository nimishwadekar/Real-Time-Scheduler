#include "sjob_queue.h"
#include "sjob.h"
#include "vector.h"
#include <stdlib.h>

#include <stdio.h>
struct SJobQueue
{
	size_t size, front;
	VectorSJobPtr jobs;
};

SJobQueuePtr SJobQueue_new(void)
{
	SJobQueuePtr queue = malloc(sizeof(struct SJobQueue));
	if(!queue)
	{
		return NULL;
	}

	queue->size = 0;
	queue->front = 0;
	queue->jobs = VectorSJob_new(); // check null
	return queue;
}

void SJobQueue_delete(SJobQueuePtr queue)
{
	VectorSJob_delete(queue->jobs);
	free(queue);
}

void SJobQueue_enqueue(SJobQueuePtr queue, SJobPtr job)
{
	VectorSJob_append(queue->jobs, job);
	queue->size++;
}

void SJobQueue_dequeue(SJobQueuePtr queue)
{
	queue->front++;
}

SJobPtr SJobQueue_peek(SJobQueuePtr queue)
{
	return VectorSJob_get(queue->jobs, queue->front);
}

int SJobQueue_is_empty(SJobQueuePtr queue)
{
	return queue->front == queue->size;
}









struct SJobPriQueue
{
	size_t size;
	VectorSJobPtr jobs;
};

#define PARENT(i) ((i - 1) / 2)
#define LEFT_CHILD(i) (2 * i + 1)
#define RIGHT_CHILD(i) (2 * i + 2)

SJobPriQueuePtr SJobPriQueue_new(void)
{
	SJobPriQueuePtr queue = malloc(sizeof(struct SJobPriQueue));
	if(!queue)
	{
		return NULL;
	}

	queue->size = 0;
	queue->jobs = VectorSJob_new(); // check null
	return queue;
}

void SJobPriQueue_delete(SJobPriQueuePtr queue)
{
	VectorSJob_delete(queue->jobs);
	free(queue);
}

void SJobPriQueue_enqueue(SJobPriQueuePtr queue, SJobPtr job)
{
	VectorSJob_append(queue->jobs, job);
	size_t i = queue->size;
	queue->size += 1;

	while(i > 0 && SJob_abs_deadline(VectorSJob_get(queue->jobs, PARENT(i))) > SJob_abs_deadline(VectorSJob_get(queue->jobs, i)))
	{
		SJobPtr parent = VectorSJob_get(queue->jobs, PARENT(i));
		VectorSJob_set(queue->jobs, PARENT(i), VectorSJob_get(queue->jobs, i));
		VectorSJob_set(queue->jobs, i, parent);

		i = PARENT(i);
	}
}

static void SJobPriQueue_min_heapify(SJobPriQueuePtr queue, size_t index)
{
	size_t left = LEFT_CHILD(index);
	size_t right = RIGHT_CHILD(index);
	size_t smallest = index;
	if(left < queue->size && SJob_abs_deadline(VectorSJob_get(queue->jobs, left)) < SJob_abs_deadline(VectorSJob_get(queue->jobs, index)))
	{
		smallest = left;
	}
	if(right < queue->size && SJob_abs_deadline(VectorSJob_get(queue->jobs, right)) < SJob_abs_deadline(VectorSJob_get(queue->jobs, smallest)))
	{
		smallest = right;
	}
	if(smallest != index)
	{
		SJobPtr smallest_job = VectorSJob_get(queue->jobs, smallest);
		VectorSJob_set(queue->jobs, smallest, VectorSJob_get(queue->jobs, index));
		VectorSJob_set(queue->jobs, index, smallest_job);
		SJobPriQueue_min_heapify(queue, smallest);
	}
}

void SJobPriQueue_dequeue(SJobPriQueuePtr queue)
{
	if(queue->size == 0)
	{
		return;
	}
	if(queue->size == 1)
	{
		//SJob_delete(VectorSJob_remove_last(queue->jobs));
		VectorSJob_remove_last(queue->jobs);
		queue->size = 0;
		return;
	}

	//SJob_delete(VectorSJob_get(queue->jobs, 0));
	VectorSJob_set(queue->jobs, 0, VectorSJob_remove_last(queue->jobs));
	queue->size -= 1;
	SJobPriQueue_min_heapify(queue, 0);
}

SJobPtr SJobPriQueue_peek(SJobPriQueuePtr queue)
{
	return VectorSJob_get(queue->jobs, 0);
}

int SJobPriQueue_is_empty(SJobPriQueuePtr queue)
{
	return queue->size == 0;
}

int SJobPriQueue_try_enqueue(SJobPriQueuePtr queue, SJobPtr job, double total_slack)
{
	for(size_t i = 0; i < queue->size; i++)
	{
		if(SJob_abs_deadline(VectorSJob_get(queue->jobs, i)) <= SJob_abs_deadline(job))
		{
			total_slack -= SJob_exec_time(VectorSJob_get(queue->jobs, i));
		}
	}
	if(total_slack < SJob_exec_time(job))
	{
		return 0;
	}

	for(size_t i = 0; i < queue->size; i++)
	{
		if(SJob_abs_deadline(VectorSJob_get(queue->jobs, i)) > SJob_abs_deadline(job))
		{
			SJobPtr later_job = VectorSJob_get(queue->jobs, i);
			if(SJob_slack(later_job) - SJob_exec_time(job) < SJob_exec_time(later_job))
			{
				return 0;
			}
		}
	}

	for(size_t i = 0; i < queue->size; i++)
	{
		if(SJob_abs_deadline(VectorSJob_get(queue->jobs, i)) > SJob_abs_deadline(job))
		{
			SJobPtr later_job = VectorSJob_get(queue->jobs, i);
			SJob_set_slack(later_job, SJob_slack(later_job) - SJob_exec_time(job));
		}
	}

	SJob_set_slack(job, total_slack);
	SJobPriQueue_enqueue(queue, job);
	return 1;
}