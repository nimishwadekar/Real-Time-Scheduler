#ifndef TASK_H
#define TASK_H 1

#include <stddef.h>


/********************************************************
 * 					PERIODIC TASK 						*
 ********************************************************/

// Type that represents a periodic task.
typedef struct PTask *PTaskPtr;


// Allocates and returns a pointer to a PTask on the heap.
PTaskPtr PTask_new(unsigned phase, unsigned period, 
	unsigned execution_time, unsigned relative_deadline);


// Frees a heap-allocated PTask.
void PTask_delete(PTaskPtr task);

unsigned PTask_get_phase(PTaskPtr task);
unsigned PTask_get_period(PTaskPtr task);
unsigned PTask_get_execution_time(PTaskPtr task);
unsigned PTask_get_relative_deadline(PTaskPtr task);



/********************************************************
 * 					PERIODIC JOB  						*
 ********************************************************/

// Type that represents a periodic job.
typedef struct PJob *PJobPtr;


// Allocates and returns 



/********************************************************
 * 					PERIODIC TASK ARRAY					*
 ********************************************************/

// Type that represents an array of PTasks.
typedef struct PTaskArray *PTaskArrayPtr;


// Reads periodic tasks from the file named "file_name", allocates enough memory on the heap for an array and writes it to the array.
// The array pointer passed points to the array on the heap.
// If the task count read is less than the tasks provided, only "task count" tasks are read.
// If the task count read is greater than the tasks provided, all provided tasks are read and the function warns the user.
// Returns 0 on success and -1 on failure.
int read_periodic_tasks(const char *file_name, PTaskArrayPtr *array);

// Allocates a task array on the heap.
PTaskArrayPtr PTaskArray_new(size_t size);

// Frees the array and its contents (including the tasks being pointed to).
void PTaskArray_delete(PTaskArrayPtr array);


size_t PTaskArray_get_size(PTaskArrayPtr array);


PTaskPtr PTaskArray_get(PTaskArrayPtr array, size_t index);


int PTaskArray_set(PTaskArrayPtr array, size_t index, PTaskPtr task);


#endif /* TASK_H */