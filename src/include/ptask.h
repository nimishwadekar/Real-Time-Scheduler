#ifndef TASK_H
#define TASK_H 1

#include <stddef.h>


/********************************************************
 * 					PERIODIC TASK 						*
 ********************************************************/

// Type that represents a periodic task.
typedef struct PTask *PTaskPtr;


// Allocates and returns a pointer to a PTask on the heap.
PTaskPtr PTask_new(int phase, int period, 
	double execution_time, int relative_deadline);


// Frees a heap-allocated PTask.
void PTask_delete(PTaskPtr task);

int PTask_get_phase(PTaskPtr task);
int PTask_get_period(PTaskPtr task);
double PTask_get_execution_time(PTaskPtr task);
int PTask_get_relative_deadline(PTaskPtr task);

// job_number starts from 0.
int PTask_get_release_time(PTaskPtr task, int job_number);
int PTask_get_absolute_deadline(PTaskPtr task, int job_number);


struct FramePJobEntry
{
    int valid; // is entry valid?
    int task;
    int job;
    double exec_time; // in current frame.
};


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


// Assumes at least 1 periodic task.
int PTaskArray_get_hyper_period(PTaskArrayPtr array);


// Returns decreasing order of possible frames one at a time. Returns 0 on no more possible frames.
// Assumes at least 1 periodic task.
int PTaskArray_get_next_possible_frame_size(PTaskArrayPtr array);


size_t PTaskArray_get_jobs_per_hyper_period(PTaskArrayPtr array, size_t job_counts[]);


#endif /* TASK_H */