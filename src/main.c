#include <stdio.h>
#include "ptask.h"
#include "read_write.h"
#include "psched.h"

int main(int argc, char *argv[])
{
	printf("\n");

	// Check args and use file names.

	int retval = schedule_periodic_tasks("periodicTasks.txt", "periodicSchedule.txt");
	if(retval != 0)
	{
		return retval;
	}
	

	return 0;
}