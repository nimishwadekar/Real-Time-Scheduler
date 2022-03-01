#ifndef READ_WRITE_H
#define READ_WRITE_H 1

#include "ptask.h"

// Reads periodic tasks from the file named "file_name", allocates enough memory on the heap for an array and writes it to the array.
// The array pointer passed points to the array on the heap.
// If the task count read is less than the tasks provided, only "task count" tasks are read.
// If the task count read is greater than the tasks provided, all provided tasks are read and the function warns the user.
// Returns 0 on success and -1 on failure.
int read_periodic_tasks(const char *file_name, PTaskArrayPtr *array);

// first line - frame size <space> frame count.
// Then frame list. Entry = job count <space> list of jobs.
// Job = task number <comma> job instance number <comma> execution time in frame
int write_periodic_schedule(const char *file_name, int frame_size, int frame_count, size_t total_jobs, struct FramePJobEntry entries[frame_count][total_jobs]);

#endif /* READ_WRITE_H */