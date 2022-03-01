#include "scheduler.h"
#include "pslice.h"
#include <stdlib.h>
#include <stdio.h>

struct Scheduler
{
	// General

	// Each time step moves forward time by one frame.
	unsigned long time;
	unsigned long current_frame;
	double frame_progress, slack_left;

	// Periodic jobs

	size_t frame_size, frame_count;
	PSliceArrayPtr *pslice_arrays;
	double *slacks;

	// Aperiodic jobs

	// Sporadic jobs
};


SchedulerPtr Scheduler_new(void)
{
	SchedulerPtr scheduler = malloc(sizeof(struct Scheduler));
	if(!scheduler)
	{
		return NULL;
	}

	return scheduler;
}

// tbc
void Scheduler_delete(SchedulerPtr scheduler)
{
	free(scheduler);
}


int Scheduler_load_periodic_schedule(SchedulerPtr scheduler, const char *file_name)
{
	FILE *file = fopen(file_name, "r");
	if(!file)
	{
		perror("Scheduler_load_periodic_schedule(): fopen() failed");
		return -1;
	}

	fscanf(file, "%lu %lu\n", &scheduler->frame_size, &scheduler->frame_count);

	scheduler->pslice_arrays = malloc(scheduler->frame_count * sizeof(PSliceArrayPtr));
	if(!scheduler->pslice_arrays)
	{
		perror("Scheduler_load_periodic_schedule(): scheduler->pslice_arrays allocation failed");
		return -1;
	}

	scheduler->slacks = malloc(scheduler->frame_count * sizeof(double));
	if(!scheduler->slacks)
	{
		perror("Scheduler_load_periodic_schedule(): scheduler->slacks allocation failed");
		return -1;
	}

	for(size_t i = 0; i < scheduler->frame_count; i++)
	{
		size_t slice_count;
		fscanf(file, "%lu ", &slice_count);

		scheduler->pslice_arrays[i] = PSliceArray_new(slice_count);
		if(!scheduler->pslice_arrays[i])
		{
			perror("Scheduler_load_periodic_schedule(): PSliceArray allocation failed");
			return -1;
		}

		double slack = scheduler->frame_size;
		for(size_t j = 0; j < slice_count; j++)
		{
			int task, job;
			double slice_time;
			fscanf(file, "%d,%d,%lg ", &task, &job, &slice_time);

			PSliceArray_set(scheduler->pslice_arrays[i], j, PSlice_new(task, job, slice_time));
			slack -= slice_time;
		}
		fscanf(file, "\n");
		scheduler->slacks[i] = slack;
	}
	fclose(file);
	return 0;
}




void Scheduler_cyclic_executive(SchedulerPtr scheduler, SchedulePtr schedule)
{
	scheduler->time = 0;
	scheduler->current_frame = 0;
	scheduler->frame_progress = 0;
	scheduler->slack_left = scheduler->slacks[0];

	Schedule_set_frame_size(schedule, scheduler->frame_size);

	int n = 10;
	while(n--)
	{
		// Assume each iteration is a timer interrupt.
		// Assume no frame overruns and no jitter.

		// TODO: Run acceptance tests on sporadic job queue to accept them.


		PSliceArrayPtr pslices = scheduler->pslice_arrays[scheduler->current_frame];
		size_t pslice_index = 0, pslice_count = PSliceArray_size(pslices);

		while(1)
		{
			// TODO: Update aperiodic job queue.


			// TODO: If no accepted sporadic jobs, but aperiodic jobs, execute until queue empty or no slack.


			// TODO: If no periodic job slices, break.
			if(pslice_index == pslice_count)
			{
				break;
			}


			// TODO: Execute 1 periodic job slice.
			PSlicePtr pslice = PSliceArray_get(pslices, pslice_index);
			//printf("%d %d %g\n", PSlice_task(pslice), PSlice_job(pslice), PSlice_slice_time(pslice));
			Schedule_add_entry(schedule, PSlice_slice_time(pslice), JOB_TYPE_PERIODIC, PSlice_job(pslice), PSlice_task(pslice));
			scheduler->frame_progress += PSlice_slice_time(pslice);
			pslice_index++;
		}

		// TODO: If accepted sporadic jobs, execute until queue empty or no slack.


		// TODO: If no more jobs to do and slack left, idle.
		if(scheduler->frame_progress < scheduler->frame_size)
		{
			Schedule_add_entry(schedule, scheduler->frame_size - scheduler->frame_progress, 
				JOB_TYPE_IDLE, 0, 0);
		}


		// TODO: Update scheduler state.
		scheduler->time++;
		scheduler->current_frame = scheduler->time % scheduler->frame_count;
		scheduler->frame_progress = 0;
		scheduler->slack_left = scheduler->slacks[scheduler->current_frame];

		//printf("\n");
	}
}