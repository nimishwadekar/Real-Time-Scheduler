#include "sjob.h"
#include <stdlib.h>

struct SJob
{
	int job_no;
	double arrival;
	double exec_time;
	double abs_deadline;
	double slack;
};


SJobPtr SJob_new(int job_no, double arrival, double exec_time, double abs_deadline)
{
	SJobPtr job = malloc(sizeof(struct SJob));
	if(!job)
	{
		return NULL;
	}

	job->job_no = job_no;
	job->arrival = arrival;
	job->exec_time = exec_time;
	job->abs_deadline = abs_deadline;
	job->slack = -1; // not yet accepted into queue.
	return job;
}

void SJob_delete(SJobPtr job)
{
	free(job);
}


int SJob_number(SJobPtr job)
{
	return job->job_no;
}

double SJob_arrival(SJobPtr job)
{
	return job->arrival;
}

double SJob_exec_time(SJobPtr job)
{
	return job->exec_time;
}

void SJob_set_exec_time(SJobPtr job, double exec_time)
{
	job->exec_time = exec_time;
}

double SJob_abs_deadline(SJobPtr job)
{
	return job->abs_deadline;
}

double SJob_slack(SJobPtr job)
{
	return job->slack;
}

void SJob_set_slack(SJobPtr job, double slack)
{
	job->slack = slack;
}