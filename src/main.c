#include <stdio.h>
#include "read_write.h"
#include "scheduler.h"

int main(int argc, char *argv[])
{
	printf("\n");

	if(argc != 4 && argc != 5)
	{
		fprintf(stderr, "Invalid number of arguments\n");
		return -1;
	}

	int retval = schedule_periodic_tasks(argv[1], "periodicSchedule.txt");
	if(retval != 0)
	{
		return retval;
	}
	
	SchedulerPtr scheduler = Scheduler_new();
	if(!scheduler)
	{
		fprintf(stderr, "main(): Scheduler_new() failed\n");
		return -1;
	}

	if(Scheduler_load_periodic_schedule(scheduler, "periodicSchedule.txt") != 0)
	{
		fprintf(stderr, "main(): Scheduler_load_periodic_schedule() failed\n");
		Scheduler_delete(scheduler);
		return -1;
	}

	if(Scheduler_load_aperiodic_schedule(scheduler, argv[2]) != 0)
	{
		fprintf(stderr, "main(): Scheduler_load_aperiodic_schedule() failed\n");
		Scheduler_delete(scheduler);
		return -1;
	}

	if(Scheduler_load_sporadic_schedule(scheduler, argv[3]) != 0)
	{
		fprintf(stderr, "main(): Scheduler_load_sporadic_schedule() failed\n");
		Scheduler_delete(scheduler);
		return -1;
	}

	SchedulePtr schedule = Schedule_new();
	if(!schedule)
	{
		fprintf(stderr, "main(): Schedule_new() failed\n");
		return -1;
	}

	Scheduler_cyclic_executive(scheduler, schedule);
	Schedule_fprint(schedule, stdout);
	if(argc == 5)
	{
		FILE *output_file = fopen(argv[4], "w");
		Schedule_fprint(schedule, output_file);
		fclose(output_file);
	}

	Scheduler_delete(scheduler);
	Schedule_delete(schedule);

	return 0;
}