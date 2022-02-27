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
		total_utilization += (double) PTask_get_execution_time(task) / PTask_get_period(task);
	}
	return total_utilization;
}


unsigned PTaskArray_get_hyper_period(PTaskArrayPtr array)
{
	unsigned hyper_period = PTask_get_period(array->array[0]);
	for(size_t i = 1; i < array->size; i++)
	{
		unsigned period = PTask_get_period(array->array[i]);
		hyper_period = period / gcd(hyper_period, period) * hyper_period;
	}
	return hyper_period;
}