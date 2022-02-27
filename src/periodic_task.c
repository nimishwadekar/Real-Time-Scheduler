#include "ptask.h"
#include <stdlib.h>
#include <stdio.h>

struct PTask
{
	unsigned phase;
	unsigned period;
	unsigned execution_time;
	unsigned relative_deadline;
};


PTaskPtr PTask_new(unsigned phase, unsigned period, 
	unsigned execution_time, unsigned relative_deadline)
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


unsigned PTask_get_phase(PTaskPtr task)
{
	return task->phase;
}


unsigned PTask_get_period(PTaskPtr task)
{
	return task->period;
}


unsigned PTask_get_execution_time(PTaskPtr task)
{
	return task->execution_time;
}


unsigned PTask_get_relative_deadline(PTaskPtr task)
{
	return task->relative_deadline;
}