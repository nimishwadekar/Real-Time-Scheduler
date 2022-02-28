#include "ptask.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>

struct PTaskArray
{
	PTaskPtr *array;
	size_t size;
};


PTaskArrayPtr PTaskArray_new(size_t size)
{
	PTaskArrayPtr array = malloc(sizeof(struct PTaskArray));
	if(!array)
	{
		perror("PTaskArray_new(): malloc() failed");
		return NULL;
	}

	array->size = size;
	array->array = malloc(size * sizeof(PTaskPtr));
	if(!array->array)
	{
		perror("PTaskArray_new(): malloc() failed");
		free(array);
		return NULL;
	}

	return array;
}


void PTaskArray_delete(PTaskArrayPtr array)
{
	for(size_t i = 0; i < array->size; i++) free(array->array[i]);
	free(array->array);
	free(array);
}


size_t PTaskArray_get_size(PTaskArrayPtr array)
{
	return array->size;
}


PTaskPtr PTaskArray_get(PTaskArrayPtr array, size_t index)
{
	if(index >= array->size)
	{
		fprintf(stderr, "PTaskArray_get(): out of bounds");
		return NULL;
	}

	return array->array[index];
}


int PTaskArray_set(PTaskArrayPtr array, size_t index, PTaskPtr task)
{
	if(index >= array->size)
	{
		fprintf(stderr, "PTaskArray_get(): out of bounds");
		return -1;
	}

	array->array[index] = task;
	return 0;
}


double PTaskArray_get_utilization(PTaskArrayPtr array)
{
	double total_utilization = 0.0;
	for(size_t i = 0; i < array->size; i++)
	{
		PTaskPtr task = array->array[i];
		total_utilization += PTask_get_execution_time(task) / PTask_get_period(task);
	}
	return total_utilization;
}


int PTaskArray_get_hyper_period(PTaskArrayPtr array)
{
	static PTaskArrayPtr cached_array = NULL;
	static int cached_hyper_period = 0;

	if(array == cached_array)
	{
		return cached_hyper_period;
	}

	int hyper_period = PTask_get_period(array->array[0]);
	for(size_t i = 1; i < array->size; i++)
	{
		int period = PTask_get_period(array->array[i]);
		hyper_period = period / gcd(hyper_period, period) * hyper_period;
	}
	cached_array = array;
	cached_hyper_period = hyper_period;
	return hyper_period;
}


int PTaskArray_get_next_possible_frame_size(PTaskArrayPtr array)
{
	static PTaskArrayPtr cached_array = NULL;
	static int frame_size = -1;
	int hyper_period = PTaskArray_get_hyper_period(array);
	if(cached_array != array)
	{
		cached_array = array;
		frame_size = hyper_period;
	}

	for( ; frame_size > 0; frame_size--)
	{
		if(hyper_period % frame_size != 0)
		{
			continue;
		}

		int valid = 1;
		for(size_t i = 0; i < array->size; i++)
		{
			if(2 * frame_size - gcd(frame_size, PTask_get_period(array->array[i])) > PTask_get_relative_deadline(array->array[i]))
			{
				valid = 0;
				break;
			}
		}
		if(valid)
		{
			int returned_frame_size = frame_size;
			frame_size--;
			return returned_frame_size;
		}
	}

	return 0;
}


size_t PTaskArray_get_jobs_per_hyper_period(PTaskArrayPtr array, size_t job_counts[])
{
	int hyper_period = PTaskArray_get_hyper_period(array);
	size_t total_jobs = 0;
	for(size_t i = 0; i < array->size; i++)
	{
		total_jobs += hyper_period / PTask_get_period(array->array[i]);
		if(job_counts) job_counts[i] = hyper_period / PTask_get_period(array->array[i]);
	}
	return total_jobs;
}