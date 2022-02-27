#include <stdio.h>
#include "ptask.h"
#include "read.h"


int main(int argc, char *argv[])
{
	printf("\n");

	// Check args and use file names.

	char *file_name = "periodic.txt";

	PTaskArrayPtr array;
	if(read_periodic_tasks(file_name, &array) == -1)
	{
		fprintf(stderr, "read_periodic_tasks() failed\n");
		return -1;
	}

	double utilization = PTaskArray_get_utilization(array);
	if(utilization > 1.0)
	{
		printf("Total utilization greater than 1\nNo schedule possible\n");
		return 1;
	}

	if(utilization > 0.0)
	{
		unsigned hyper_period = PTaskArray_get_hyper_period(array);

		// get frame size and periodic schedule using INF.
	}

	return 0;
}