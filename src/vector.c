#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

struct VectorInt
{
	int *array;
	size_t count, capacity;
};

VectorIntPtr VectorInt_new(void)
{
	VectorIntPtr vec = malloc(sizeof(struct VectorInt));
	if(!vec)
	{
		return NULL;
	}

	vec->array = malloc(8 * sizeof(int));
	if(!vec->array)
	{
		free(vec);
		return NULL;
	}
	vec->count = 0;
	vec->capacity = 8;
	return vec;
}

void VectorInt_delete(VectorIntPtr vector)
{
	free(vector->array);
	free(vector);
}

size_t VectorInt_size(VectorIntPtr vector)
{
	return vector->count;
}

int VectorInt_get(VectorIntPtr vector, size_t index)
{
	if(index >= vector->count)
	{
		fprintf(stderr, "VectorInt_get(): out of bounds\n");
		return -1;
	}
	return vector->array[index];
}

void VectorInt_set(VectorIntPtr vector, size_t index, int element)
{
	if(index >= vector->count)
	{
		fprintf(stderr, "VectorInt_get(): out of bounds\n");
		return;
	}
	vector->array[index] = element;
}

void VectorInt_append(VectorIntPtr vector, int element)
{
	if(vector->count < vector->capacity)
	{
		vector->array[vector->count] = element;
		vector->count++;
		return;
	}

	vector->array = realloc(vector->array, vector->capacity * 2 * sizeof(int));
	vector->capacity *= 2;
	VectorInt_append(vector, element);
}
















struct VectorDouble
{
	double *array;
	size_t count, capacity;
};

VectorDoublePtr VectorDouble_new(void)
{
	VectorDoublePtr vec = malloc(sizeof(struct VectorDouble));
	if(!vec)
	{
		return NULL;
	}

	vec->array = malloc(8 * sizeof(double));
	if(!vec->array)
	{
		free(vec);
		return NULL;
	}
	vec->count = 0;
	vec->capacity = 8;
	return vec;
}

void VectorDouble_delete(VectorDoublePtr vector)
{
	free(vector->array);
	free(vector);
}

size_t VectorDouble_size(VectorDoublePtr vector)
{
	return vector->count;
}

double VectorDouble_get(VectorDoublePtr vector, size_t index)
{
	if(index >= vector->count)
	{
		fprintf(stderr, "VectorDouble_get(): out of bounds\n");
		return -1;
	}
	return vector->array[index];
}

void VectorDouble_set(VectorDoublePtr vector, size_t index, double element)
{
	if(index >= vector->count)
	{
		fprintf(stderr, "VectorDouble_get(): out of bounds\n");
		return;
	}
	vector->array[index] = element;
}

void VectorDouble_append(VectorDoublePtr vector, double element)
{
	if(vector->count < vector->capacity)
	{
		vector->array[vector->count] = element;
		vector->count++;
		return;
	}

	vector->array = realloc(vector->array, vector->capacity * 2 * sizeof(double));
	vector->capacity *= 2;
	VectorDouble_append(vector, element);
}
















struct VectorSJob
{
	SJobPtr *array;
	size_t count, capacity;
};

VectorSJobPtr VectorSJob_new(void)
{
	VectorSJobPtr vec = malloc(sizeof(struct VectorSJob));
	if(!vec)
	{
		return NULL;
	}

	vec->array = malloc(8 * sizeof(SJobPtr));
	if(!vec->array)
	{
		free(vec);
		return NULL;
	}
	vec->count = 0;
	vec->capacity = 8;
	return vec;
}

void VectorSJob_delete(VectorSJobPtr vector)
{
	free(vector->array);
	free(vector);
}

size_t VectorSJob_size(VectorSJobPtr vector)
{
	return vector->count;
}

SJobPtr VectorSJob_get(VectorSJobPtr vector, size_t index)
{
	if(index >= vector->count)
	{
		fprintf(stderr, "VectorSJob_get(): out of bounds\n");
		return NULL;
	}
	return vector->array[index];
}

void VectorSJob_set(VectorSJobPtr vector, size_t index, SJobPtr element)
{
	if(index >= vector->count)
	{
		fprintf(stderr, "VectorSJob_get(): out of bounds\n");
		return;
	}
	vector->array[index] = element;
}

void VectorSJob_append(VectorSJobPtr vector, SJobPtr element)
{
	if(vector->count < vector->capacity)
	{
		vector->array[vector->count] = element;
		vector->count++;
		return;
	}

	vector->array = realloc(vector->array, vector->capacity * 2 * sizeof(SJobPtr));
	vector->capacity *= 2;
	VectorSJob_append(vector, element);
}

SJobPtr VectorSJob_remove_last(VectorSJobPtr vector)
{
	if(vector->count == 0)
	{
		return NULL;
	}

	vector->count -= 1;
	return vector->array[vector->count];
}