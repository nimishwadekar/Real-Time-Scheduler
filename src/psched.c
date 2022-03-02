#include "scheduler.h"
#include "inf.h"
#include "ptask.h"
#include "read_write.h"
#include <stdio.h>
#include <string.h>

int schedule_periodic_tasks(const char *task_file_name, const char *sched_file_name)
{
	PTaskArrayPtr array;
	if(read_periodic_tasks(task_file_name, &array) == -1)
	{
		fprintf(stderr, "read_periodic_tasks() failed\n");
		return -1;
	}

	double utilization = PTaskArray_get_utilization(array);
	if(utilization > 1.0)
	{
		printf("Total utilization greater than 1\nNo schedule possible\n");
		return 1;
	}
	if(utilization <= 0.0) // No periodic task executions to schedule
	{
		if(write_periodic_schedule(sched_file_name, 0, 0, 0, NULL) != 0)
		{
			fprintf(stderr, "schedule_periodic_tasks(): write_periodic_schedule() failed\n");
			PTaskArray_delete(array);
			return -1;
		}
		return 0;
	}
	
	int hyper_period = PTaskArray_get_hyper_period(array);

	int frame_size;
	while((frame_size = PTaskArray_get_next_possible_frame_size(array)) > 0)
	{
		size_t total_jobs = PTaskArray_get_jobs_per_hyper_period(array, NULL);
		struct FramePJobEntry entries[hyper_period / frame_size][total_jobs];
		memset(entries, 0, sizeof(entries));

		if(inf_algorithm(array, frame_size, hyper_period / frame_size, total_jobs, entries) == 0)
		{
			if(write_periodic_schedule(sched_file_name, frame_size, hyper_period / frame_size, total_jobs, entries) != 0)
			{
				fprintf(stderr, "schedule_periodic_tasks(): write_periodic_schedule() failed\n");
				PTaskArray_delete(array);
				return -1;
			}
			PTaskArray_delete(array);
			return 0;
		}
	}

	printf("INF algorithm could not find a periodic schedule for any frame size\n");
	PTaskArray_delete(array);
	return 1;
}