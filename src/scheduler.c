#include "scheduler.h"
#include "ajob.h"
#include "pslice.h"
#include "sjob.h"
#include "sjob_queue.h"
#include "common.h"
#include <bits/types/time_t.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef RAND_EXEC_TIME
#include <time.h>

static inline double rand_20_to_100(double val)
{
	return TO_SCHEDULER_PRECISION((1 - (double) rand() / RAND_MAX * 0.8) * val);
}
#endif /* RAND_EXEC_TIME */


#define DEFAULT_FRAME_SIZE 1
#define DEFAULT_FRAME_COUNT 1


struct Scheduler
{
	// General
	unsigned long time; // Each time step moves forward time by one frame.
	unsigned long current_frame;
	double frame_progress, slack_left;

	// Periodic jobs
	size_t frame_size, frame_count;
	PSliceArrayPtr *pslice_arrays;
	double *slacks;

	// Aperiodic jobs
	AJobQueuePtr ajobs_all; // all jobs, backend queue used to simulate job arrival.
	AJobQueuePtr ajobs_queue;

	// Sporadic jobs
	SJobQueuePtr sjobs_all; // all jobs, backend queue used to simulate job arrival.
	SJobPriQueuePtr sjobs_accepted;
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


void Scheduler_delete(SchedulerPtr scheduler)
{
	for(size_t i = 0; i < scheduler->frame_count; i++)
	{
		PSliceArray_delete(scheduler->pslice_arrays[i]);
	}
	free(scheduler->pslice_arrays);
	free(scheduler->slacks);

	AJobQueue_delete(scheduler->ajobs_all);
	AJobQueue_delete(scheduler->ajobs_queue);

	SJobQueue_delete(scheduler->sjobs_all);
	SJobPriQueue_delete(scheduler->sjobs_accepted);

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

	if(scheduler->frame_count == 0)
	{
		scheduler->frame_count = DEFAULT_FRAME_COUNT;
		scheduler->frame_size = DEFAULT_FRAME_SIZE;
		scheduler->pslice_arrays = malloc(sizeof(PSliceArrayPtr));
		if(!scheduler->pslice_arrays)
		{
			perror("Scheduler_load_periodic_schedule(): scheduler->pslice_arrays malloc() failed");
			fclose(file);
			return -1;
		}

		scheduler->pslice_arrays[0] = PSliceArray_new(0);
		if(!scheduler->pslice_arrays[0])
		{
			perror("Scheduler_load_periodic_schedule(): PSliceArray_new() failed");
			fclose(file);
			free(scheduler->pslice_arrays);
			return -1;
		}

		scheduler->slacks = malloc(sizeof(double));
		if(!scheduler->slacks)
		{
			perror("Scheduler_load_periodic_schedule(): scheduler->slacks malloc() failed");
			fclose(file);
			PSliceArray_delete(scheduler->pslice_arrays[0]);
			free(scheduler->pslice_arrays);
			return -1;
		}

		scheduler->slacks[0] = DEFAULT_FRAME_SIZE;
		fclose(file);
		return 0;
	}

	scheduler->pslice_arrays = malloc(scheduler->frame_count * sizeof(PSliceArrayPtr));
	if(!scheduler->pslice_arrays)
	{
		perror("Scheduler_load_periodic_schedule(): scheduler->pslice_arrays malloc() failed");
		fclose(file);
		return -1;
	}

	scheduler->slacks = malloc(scheduler->frame_count * sizeof(double));
	if(!scheduler->slacks)
	{
		perror("Scheduler_load_periodic_schedule(): scheduler->slacks malloc() failed");
		fclose(file);
		free(scheduler->pslice_arrays);
		return -1;
	}

	for(size_t i = 0; i < scheduler->frame_count; i++)
	{
		size_t slice_count;
		fscanf(file, "%lu ", &slice_count);

		scheduler->pslice_arrays[i] = PSliceArray_new(slice_count);
		if(!scheduler->pslice_arrays[i])
		{
			perror("Scheduler_load_periodic_schedule(): PSliceArray_new() failed");
			fclose(file);
			free(scheduler->pslice_arrays);
			free(scheduler->slacks);
			return -1;
		}

		double slack = scheduler->frame_size;
		for(size_t j = 0; j < slice_count; j++)
		{
			int task, job;
			double slice_time;
			fscanf(file, "%d,%d,%lg ", &task, &job, &slice_time);

			PSlicePtr pslice = PSlice_new(task, job, slice_time);
			if(!pslice)
			{
				perror("Scheduler_load_periodic_schedule(): PSlice_new() failed");
				fclose(file);
				free(scheduler->pslice_arrays);
				free(scheduler->slacks);
				return -1;
			}

			PSliceArray_set(scheduler->pslice_arrays[i], j, pslice);
			slack -= slice_time;
		}
		fscanf(file, "\n");
		scheduler->slacks[i] = slack;
	}
	fclose(file);
	return 0;
}


int Scheduler_load_aperiodic_schedule(SchedulerPtr scheduler, const char *file_name)
{
	FILE *file = fopen(file_name, "r");
	if(!file)
	{
		perror("Scheduler_load_aperiodic_schedule(): fopen() failed");
		return -1;
	}

	scheduler->ajobs_all = AJobQueue_new();
	if(!scheduler->ajobs_all)
	{
		perror("Scheduler_load_aperiodic_schedule(): scheduler->ajobs_all AJobQueue_new() failed");
		fclose(file);
		return -1;
	}

	scheduler->ajobs_queue = AJobQueue_new();
	if(!scheduler->ajobs_queue)
	{
		perror("Scheduler_load_aperiodic_schedule(): scheduler->ajobs_queue AJobQueue_new() failed");
		AJobQueue_delete(scheduler->ajobs_all);
		fclose(file);
		return -1;
	}

	int job_count;
	fscanf(file, "%d\n", &job_count);
	while(job_count > 0 && !feof(file))
	{
		double release_time, exec_time;
		fscanf(file, "%lg %lg\n", &release_time, &exec_time);
		AJobQueue_enqueue(scheduler->ajobs_all, release_time, exec_time);
		job_count -= 1;
	}

	return 0;
}


int Scheduler_load_sporadic_schedule(SchedulerPtr scheduler, const char *file_name)
{
	FILE *file = fopen(file_name, "r");
	if(!file)
	{
		perror("Scheduler_load_sporadic_schedule(): fopen() failed");
		return -1;
	}

	scheduler->sjobs_all = SJobQueue_new(); // check null
	if(!scheduler->sjobs_all)
	{
		perror("Scheduler_load_sporadic_schedule(): SJobQueue_new() failed");
		fclose(file);
		return -1;
	}

	scheduler->sjobs_accepted = SJobPriQueue_new(); // check null
	if(!scheduler->sjobs_accepted)
	{
		perror("Scheduler_load_sporadic_schedule(): SJobPriQueue_new() failed");
		SJobQueue_delete(scheduler->sjobs_all);
		fclose(file);
		return -1;
	}

	int job_no = 0;
	int job_count;
	fscanf(file, "%d\n", &job_count);
	while(job_count > 0 && !feof(file))
	{
		double arrival, exec_time, abs_deadline;
		fscanf(file, "%lg %lg %lg\n", &arrival, &exec_time, &abs_deadline);
		SJobQueue_enqueue(scheduler->sjobs_all, SJob_new(job_no, arrival, exec_time, abs_deadline));
		job_no += 1;
		job_count -= 1;
	}

	return 0;
}


static void Scheduler_update_aperiodic_queue(SchedulerPtr scheduler)
{
	double cur_time = scheduler->time * scheduler->frame_size + scheduler->frame_progress;
	double release_time, exec_time;
	while(!AJobQueue_is_empty(scheduler->ajobs_all))
	{
		AJobQueue_peek(scheduler->ajobs_all, &release_time, &exec_time);
		if(cur_time >= release_time)
		{
			#ifdef RAND_EXEC_TIME
			exec_time = rand_20_to_100(exec_time);
			#endif /* RAND_EXEC_TIME */

			AJobQueue_enqueue(scheduler->ajobs_queue, release_time, exec_time);
			AJobQueue_dequeue(scheduler->ajobs_all);
		}
		else
		{
			break;
		}
	}
}

static void Scheduler_update_sporadic_queue(SchedulerPtr scheduler)
{
	double cur_time = scheduler->time * scheduler->frame_size;
	while(!SJobQueue_is_empty(scheduler->sjobs_all))
	{
		SJobPtr job = SJobQueue_peek(scheduler->sjobs_all);
		if(cur_time >= SJob_arrival(job))
		{
			double total_slack = 0;
			for(size_t frame = scheduler->current_frame; frame < (size_t) (SJob_abs_deadline(job) / scheduler->frame_size); frame++)
			{
				total_slack += scheduler->slacks[frame % scheduler->frame_count];
			}

			if(SJobPriQueue_try_enqueue(scheduler->sjobs_accepted, job, total_slack))
			{	
				#ifdef RAND_EXEC_TIME
				SJob_set_exec_time(job, rand_20_to_100(SJob_exec_time(job)));
				#endif /* RAND_EXEC_TIME */
			}
			SJobQueue_dequeue(scheduler->sjobs_all);
		}
		else
		{
			break;
		}
	}
}


static void Scheduler_execute_aperiodic_jobs(SchedulerPtr scheduler, SchedulePtr schedule)
{
	double release_time, exec_time;
	while(!AJobQueue_is_empty(scheduler->ajobs_queue) && scheduler->slack_left > 0)
	{
		int job = AJobQueue_peek(scheduler->ajobs_queue, &release_time, &exec_time);
		if(exec_time <= scheduler->slack_left)
		{
			Schedule_add_entry(schedule, exec_time, JOB_TYPE_APERIODIC, job, 0);
			AJobQueue_dequeue(scheduler->ajobs_queue);
			scheduler->slack_left -= exec_time;
			scheduler->frame_progress += exec_time;
		}
		else
		{
			Schedule_add_entry(schedule, scheduler->slack_left, JOB_TYPE_APERIODIC, job, 0);
			AJobQueue_update_exec_time(scheduler->ajobs_queue, exec_time - scheduler->slack_left);
			scheduler->frame_progress += scheduler->slack_left;
			scheduler->slack_left = 0;
		}
		Scheduler_update_aperiodic_queue(scheduler);
	}
}


static void Scheduler_execute_sporadic_jobs(SchedulerPtr scheduler, SchedulePtr schedule)
{
	while(!SJobPriQueue_is_empty(scheduler->sjobs_accepted) && scheduler->slack_left > 0)
	{
		SJobPtr job = SJobPriQueue_peek(scheduler->sjobs_accepted);
		double exec_time = SJob_exec_time(job);
		if(exec_time <= scheduler->slack_left)
		{
			Schedule_add_entry(schedule, exec_time, JOB_TYPE_SPORADIC, SJob_number(job), 0);
			SJobPriQueue_dequeue(scheduler->sjobs_accepted);
			scheduler->slack_left -= exec_time;
			scheduler->frame_progress += exec_time;
		}
		else
		{
			Schedule_add_entry(schedule, scheduler->slack_left, JOB_TYPE_SPORADIC, SJob_number(job), 0);
			SJob_set_exec_time(job, exec_time - scheduler->slack_left);
			scheduler->frame_progress += scheduler->slack_left;
			scheduler->slack_left = 0;
		}
	}
}


void Scheduler_cyclic_executive(SchedulerPtr scheduler, SchedulePtr schedule)
{
	#ifdef RAND_EXEC_TIME
	// Initialise random number generator with current time as seed.
	srand((unsigned) time(NULL));
	#endif /* RAND_EXEC_TIME */


	scheduler->time = 0;
	scheduler->current_frame = 0;
	scheduler->frame_progress = 0;

	PSliceArrayPtr pslices;
	size_t pslice_index = 0, pslice_count = 0;
	scheduler->slack_left = scheduler->slacks[0];
	pslices = scheduler->pslice_arrays[0];
	pslice_count = PSliceArray_size(pslices);

	Schedule_set_frame_size(schedule, scheduler->frame_size);

	while(1)
	{
		// Assume each iteration is a timer interrupt.
		// Assume no frame overruns and no jitter.

		// Run acceptance tests on sporadic job queue to accept them.
		if(DOUBLE_EQ(scheduler->frame_progress, 0))
		{
			Scheduler_update_sporadic_queue(scheduler);
		}		

		while(1)
		{
			// Update aperiodic job queue.
			Scheduler_update_aperiodic_queue(scheduler);

			// If no accepted sporadic jobs, but aperiodic jobs, execute until queue empty or no slack.
			if(SJobPriQueue_is_empty(scheduler->sjobs_accepted))
			{
				Scheduler_execute_aperiodic_jobs(scheduler, schedule);
			}

			// If no periodic job slices, break.
			if(pslice_index == pslice_count)
			{
				break;
			}

			// Execute 1 periodic job slice.
			PSlicePtr pslice = PSliceArray_get(pslices, pslice_index);
			double slice_time = PSlice_slice_time(pslice);

			#ifdef RAND_EXEC_TIME
			slice_time = rand_20_to_100(slice_time);
			#endif /* RAND_EXEC_TIME */

			Schedule_add_entry(schedule, slice_time, JOB_TYPE_PERIODIC, PSlice_job(pslice), PSlice_task(pslice));
			scheduler->frame_progress += slice_time;
			pslice_index++;
		}

		// If accepted sporadic jobs, execute until queue empty or no slack.
		if(!SJobPriQueue_is_empty(scheduler->sjobs_accepted) && scheduler->slack_left > 0)
		{
			Scheduler_execute_sporadic_jobs(scheduler, schedule);
			continue;
		}

		// If no more jobs to do and slack left, idle and wait for new aperiodic jobs, if any.
		if(scheduler->frame_progress < scheduler->frame_size)
		{
			long idle_start = (long) (scheduler->frame_progress * SCHEDULER_TIMESTEP_PRECISION);
			long idle_timestep = idle_start;
			long frame_end = (long) (scheduler->frame_size * SCHEDULER_TIMESTEP_PRECISION);
			for( ; idle_timestep < frame_end; idle_timestep++)
			{
				scheduler->frame_progress = (double) idle_timestep * SCHEDULER_MINIMUM_TIMESTEP;
				Scheduler_update_aperiodic_queue(scheduler);
				if(!AJobQueue_is_empty(scheduler->ajobs_queue))
				{
					break;
				}
			}
			scheduler->frame_progress = (double) idle_timestep * SCHEDULER_MINIMUM_TIMESTEP;

			Schedule_add_entry(schedule, scheduler->frame_progress - idle_start * SCHEDULER_MINIMUM_TIMESTEP, JOB_TYPE_IDLE, 0, 0);

			if(!DOUBLE_EQ(scheduler->frame_progress, scheduler->frame_size))
			{
				continue;
			}
		}

		// Update scheduler state.
		scheduler->time++;
		scheduler->current_frame = scheduler->time % scheduler->frame_count;
		scheduler->frame_progress = 0;
		scheduler->slack_left = scheduler->slacks[scheduler->current_frame];
		pslices = scheduler->pslice_arrays[scheduler->current_frame];
		pslice_index = 0;
		pslice_count = PSliceArray_size(pslices);


		// Stop writing to schedule structure if only periodic jobs (or no jobs) are left now.
		if(scheduler->current_frame == 0 && DOUBLE_EQ(scheduler->frame_progress, 0) && 
			AJobQueue_is_empty(scheduler->ajobs_all) && AJobQueue_is_empty(scheduler->ajobs_queue) &&
			SJobQueue_is_empty(scheduler->sjobs_all) && SJobPriQueue_is_empty(scheduler->sjobs_accepted))
		{
			break;
		}
	}
}