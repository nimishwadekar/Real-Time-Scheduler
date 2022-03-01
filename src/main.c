#include <stdio.h>
#include "ptask.h"
#include "read_write.h"
#include "scheduler.h"

int main(int argc, char *argv[])
{
	printf("\n");

	// Check args and use file names.

	int retval = schedule_periodic_tasks("periodicTasks.txt", "periodicSchedule.txt");
	if(retval != 0)
	{
		return retval;
	}
	
	SchedulerPtr scheduler = Scheduler_new();

	if(Scheduler_load_periodic_schedule(scheduler, "periodicSchedule.txt") != 0)
	{
		fprintf(stderr, "Load periodic table failed\n");
		Scheduler_delete(scheduler);
		return -1;
	}

	Scheduler_cyclic_executive(scheduler);

	Scheduler_delete(scheduler);

	return 0;
}