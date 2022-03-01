#include "read_write.h"
#include <stdio.h>

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

	for(size_t i = 0; i < task_count; i++)
	{
		if(feof(file))
		{
			fprintf(stderr, "read_periodic_tasks(): Periodic task count greater than list of tasks provided\n");
			return -1;
		}

		int phase, period, relative_deadline;
		double execution_time;
		fscanf(file, "%d %d %lg %d\n", &phase, &period, &execution_time, &relative_deadline);

		PTaskPtr task = PTask_new(phase, period, execution_time, relative_deadline);
		if(!task) return -1;

		PTaskArray_set(array_ptr, i, task);
	}

	*array = array_ptr;
	return 0;
}


int write_periodic_schedule(const char *file_name, int frame_size, int frame_count, size_t total_jobs, struct FramePJobEntry entries[frame_count][total_jobs])
{
	FILE *file = fopen(file_name, "w");
	if(!file)
	{
		perror("write_periodic_schedule(): open() failed");
		return -1;
	}

	fprintf(file, "%d %d\n", frame_size, frame_count);
	for(int i = 0; i < frame_count; i++)
	{
		int job_count = 0;
		for(int j = 0; j < total_jobs; j++)
		{
			if(entries[i][j].valid)
			{
				job_count++;
			}
		}
		fprintf(file, "%d ", job_count);

		for(int j = 0; j < total_jobs; j++)
		{
			struct FramePJobEntry *entry = &entries[i][j];
			if(!entry->valid)
			{
				continue;
			}

			fprintf(file, "%d,%d,%g ", entry->task, entry->job, entry->exec_time);
		}
		fprintf(file, "\n");
	}
	return 0;
}