#ifndef READ_H
#define READ_H 1

#include "ptask.h"

// Reads periodic tasks from the file named "file_name", allocates enough memory on the heap for an array and writes it to the array.
// The array pointer passed points to the array on the heap.
// If the task count read is less than the tasks provided, only "task count" tasks are read.
// If the task count read is greater than the tasks provided, all provided tasks are read and the function warns the user.
// Returns 0 on success and -1 on failure.
int read_periodic_tasks(const char *file_name, PTaskArrayPtr *array);

#endif /* READ_H */