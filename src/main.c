#include <stdio.h>
#include "ptask.h"


int read_periodic_tasks(const char *file_name, PTaskArrayPtr *array)
{
	FILE *file = fopen(file_name, "r");
	if(!file)
	{
		perror("read_periodic_tasks(): open() failed");
		return -1;
	}

	size_t task_count;
	fscanf(file, "%lu\n", &task_count);

	PTaskArrayPtr array_ptr = PTaskArray_new(task_count);

	for(unsigned i = 0; i < task_count; i++)
	{
		if(feof(file))
		{
			fprintf(stderr, "read_periodic_tasks(): Periodic task count greater than list of tasks provided\n");
			return -1;
		}

		unsigned phase, period, execution_time, relative_deadline;
		fscanf(file, "%u %u %u %u\n", &phase, &period, &execution_time, &relative_deadline);

		PTaskPtr task = PTask_new(phase, period, execution_time, relative_deadline);
		if(!task) return -1;

		PTaskArray_set(array_ptr, i, task);
	}

	*array = array_ptr;
	return 0;
}


int main(int argc, char *argv[])
{
	printf("*****************************************************\n\n");

	// Check args and use file names.

	char *file_name = "periodic.txt";

	PTaskArrayPtr array;
	if(read_periodic_tasks(file_name, &array) == -1)
	{
		fprintf(stderr, "read_periodic_tasks() failed\n");
		return -1;
	}

	for(size_t i = 0; i < PTaskArray_get_size(array); i++)
	{
		PTaskPtr task = PTaskArray_get(array, i);
		printf("%u %u %u %u\n", PTask_get_phase(task), PTask_get_period(task), PTask_get_execution_time(task), PTask_get_relative_deadline(task));
	}
	

	printf("\n*****************************************************\n");
	return 0;
}