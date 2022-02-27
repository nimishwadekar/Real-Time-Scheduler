#include <stdio.h>
#include "ptask.h"
#include "read.h"


int main(int argc, char *argv[])
{
	printf("*****************************************************\n\n");

	// Check args and use file names.

	char *file_name = "periodic.txt";

	PTaskArrayPtr array;
	if(read_periodic_tasks(file_name, &array) == -1)
	{
		fprintf(stderr, "read_periodic_tasks() failed\n");
		return -1;
	}

	printf("%g\n", PTaskArray_get_utilization(array));	

	printf("\n*****************************************************\n");
	return 0;
}