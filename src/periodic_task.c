#include "ptask.h"
#include <stdlib.h>
#include <stdio.h>

struct PTask
{
	int phase;
	int period;
	double execution_time;
	int relative_deadline;
};


PTaskPtr PTask_new(int phase, int period, 
	double execution_time, int relative_deadline)
{
	PTaskPtr pTask = malloc(sizeof(struct PTask));
	if(!pTask)
	{
		perror("Periodic task creation failed");
		return NULL;
	}

	pTask->phase = phase;
	pTask->period = period;
	pTask->execution_time = execution_time;
	pTask->relative_deadline = relative_deadline;

	return pTask;
}


void PTask_delete(PTaskPtr task)
{
	free(task);
}


int PTask_get_phase(PTaskPtr task)
{
	return task->phase;
}


int PTask_get_period(PTaskPtr task)
{
	return task->period;
}


double PTask_get_execution_time(PTaskPtr task)
{
	return task->execution_time;
}


int PTask_get_relative_deadline(PTaskPtr task)
{
	return task->relative_deadline;
}


int PTask_get_release_time(PTaskPtr task, int job_number)
{
	return task->period * job_number + task->phase;
}


int PTask_get_absolute_deadline(PTaskPtr task, int job_number)
{
	return task->period * job_number + task->phase + task->relative_deadline;
}