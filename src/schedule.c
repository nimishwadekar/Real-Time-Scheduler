#include "scheduler.h"
#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

struct Schedule
{
	size_t size, frame_size;
	VectorDoublePtr slice_times;
	VectorIntPtr job_types;
	VectorIntPtr jobs;

	// Only for periodic jobs. 0 for all others.
	VectorIntPtr tasks;
};

SchedulePtr Schedule_new(void)
{
	SchedulePtr sched = malloc(sizeof(struct Schedule));
	if(!sched)
	{
		return NULL;
	}

	sched->slice_times = VectorDouble_new();
	if(!sched->slice_times)
	{
		free(sched);
		return NULL;
	}

	sched->job_types = VectorInt_new();
	if(!sched->job_types)
	{
		VectorDouble_delete(sched->slice_times);
		free(sched);
		return NULL;
	}

	sched->jobs = VectorInt_new();
	if(!sched->jobs)
	{
		VectorInt_delete(sched->job_types);
		VectorDouble_delete(sched->slice_times);
		free(sched);
		return NULL;
	}

	sched->tasks = VectorInt_new();
	if(!sched->tasks)
	{
		VectorInt_delete(sched->jobs);
		VectorInt_delete(sched->job_types);
		VectorDouble_delete(sched->slice_times);
		free(sched);
		return NULL;
	}

	return sched;
}


void Schedule_delete(SchedulePtr schedule)
{
	VectorInt_delete(schedule->tasks);
	VectorInt_delete(schedule->jobs);
	VectorInt_delete(schedule->job_types);
	VectorDouble_delete(schedule->slice_times);
	free(schedule);
}


size_t Schedule_size(SchedulePtr schedule)
{
	return schedule->size;
}


void Schedule_set_frame_size(SchedulePtr schedule, size_t frame_size)
{
	schedule->frame_size = frame_size;
}


void Schedule_add_entry(SchedulePtr schedule, double slice_time, enum JobType job_type, int job, int task_if_periodic)
{
	VectorDouble_append(schedule->slice_times, slice_time);
	VectorInt_append(schedule->job_types, job_type);
	VectorInt_append(schedule->jobs, job);
	if(job_type == JOB_TYPE_PERIODIC)
	{
		VectorInt_append(schedule->tasks, task_if_periodic);
	}
	else
	{
		VectorInt_append(schedule->tasks, 0);
	}
	
	schedule->size += 1;
}


void Schedule_get_entry(SchedulePtr schedule, size_t index, double *slice_time, enum JobType *job_type, int *job, int *task_if_periodic)
{
	*slice_time = VectorDouble_get(schedule->slice_times, index);
	*job_type = VectorInt_get(schedule->job_types, index);
	*job = VectorInt_get(schedule->jobs, index);
	if(*job_type == JOB_TYPE_PERIODIC)
	{
		*task_if_periodic = VectorInt_get(schedule->tasks, index);
	}
}


void Schedule_fprint(SchedulePtr schedule, FILE *stream)
{
	double time_since_frame_start = 0, time_since_start = 0;
	for(size_t i = 0; i < Schedule_size(schedule); i++)
	{
		double time;
		enum JobType type;
		int job, task;
		Schedule_get_entry(schedule, i, &time, &type, &job, &task);

		if(type == JOB_TYPE_PERIODIC)
		{
			fprintf(stream, "%3g %9s %d:%d\n", time_since_start, JobType_str(type), task, job);
		}
		else if(type == JOB_TYPE_IDLE)
		{
			fprintf(stream, "%3g %9s\n", time_since_start, JobType_str(type));
		}
		else
		{
			fprintf(stream, "%3g %9s %d\n", time_since_start, JobType_str(type), job);
		}

		time_since_start += time;
		time_since_frame_start += time;
		if(time_since_frame_start >= schedule->frame_size)
		{
			fprintf(stream, "\n");
			time_since_frame_start = 0;
		}	
	}
}


static const char* JOB_TYPE_STR[] =
{
	"PERIODIC",
	"APERIODIC",
	"SPORADIC",
	"IDLE"
};

const char* JobType_str(enum JobType job_type)
{
	return JOB_TYPE_STR[job_type];
}