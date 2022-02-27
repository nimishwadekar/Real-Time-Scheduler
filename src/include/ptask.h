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

// Allocates a task array on the heap.
PTaskArrayPtr PTaskArray_new(size_t size);

// Frees the array and its contents (including the tasks being pointed to).
void PTaskArray_delete(PTaskArrayPtr array);


size_t PTaskArray_get_size(PTaskArrayPtr array);


PTaskPtr PTaskArray_get(PTaskArrayPtr array, size_t index);


int PTaskArray_set(PTaskArrayPtr array, size_t index, PTaskPtr task);


double PTaskArray_get_utilization(PTaskArrayPtr array);


#endif /* TASK_H */