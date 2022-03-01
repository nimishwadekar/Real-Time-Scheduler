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
	double current_slack;

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




void Scheduler_cyclic_executive(SchedulerPtr scheduler)
{
	scheduler->time = 0;
	scheduler->current_frame = 0;
	scheduler->current_slack = scheduler->slacks[0];

	while(1)
	{
		// Assume each iteration is a timer interrupt.
		// Assume no frame overruns and all slices are released immediately at frame start.

		// TODO: Run acceptance tests on sporadic job queue to accept them.


		PSliceArrayPtr pslices = scheduler->pslice_arrays[scheduler->current_frame];
		size_t slice_index = 0, slice_count = PSliceArray_size(pslices);

		while(1)
		{
			// TODO: Update aperiodic job queue.


			// TODO: If no accepted sporadic jobs, but aperiodic jobs, execute until queue empty or no slack.


			// TODO: If no periodic job slices, break.


			// TODO: Execute 1 periodic job slice.
			

		}

		// TODO: If accepted sporadic jobs, execute until queue empty or no slack.


		// TODO: Update scheduler state.
	}
}