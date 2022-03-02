#include "pslice.h"
#include <stdlib.h>
#include <stdio.h>

struct PSlice
{
	int task;
	int job;
	double slice_time;
};

PSlicePtr PSlice_new(int task, int job, double slice_time)
{
	PSlicePtr slice = malloc(sizeof(struct PSlice));
	if(!slice)
	{
		return NULL;
	}

	slice->task = task;
	slice->job = job;
	slice->slice_time = slice_time;

	return slice;
}


void PSlice_delete(PSlicePtr slice)
{
	free(slice);
}


int PSlice_task(PSlicePtr slice)
{
	return slice->task;
}


int PSlice_job(PSlicePtr slice)
{
	return slice->job;
}


double PSlice_slice_time(PSlicePtr slice)
{
	return slice->slice_time;
}





struct PSliceArray
{
	PSlicePtr *array;
	size_t size;
};


PSliceArrayPtr PSliceArray_new(size_t size)
{
	PSliceArrayPtr array = malloc(sizeof(struct PSliceArray));
	if(!array)
	{
		perror("PSliceArray_new(): malloc() failed");
		return NULL;
	}

	array->size = size;
	array->array = malloc(size * sizeof(PSlicePtr));
	if(!array->array)
	{
		perror("PSliceArray_new(): malloc() failed");
		free(array);
		return NULL;
	}

	return array;
}


void PSliceArray_delete(PSliceArrayPtr array)
{
	for(size_t i = 0; i < array->size; i++) free(array->array[i]);
	free(array->array);
	free(array);
}


size_t PSliceArray_size(PSliceArrayPtr array)
{
	return array->size;
}


PSlicePtr PSliceArray_get(PSliceArrayPtr array, size_t index)
{
	if(index >= array->size)
	{
		fprintf(stderr, "PSliceArray_get(): out of bounds");
		return NULL;
	}

	return array->array[index];
}


int PSliceArray_set(PSliceArrayPtr array, size_t index, PSlicePtr slice)
{
	if(index >= array->size)
	{
		fprintf(stderr, "PSliceArray_get(): out of bounds");
		return -1;
	}

	array->array[index] = slice;
	return 0;
}