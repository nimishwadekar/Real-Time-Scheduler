#ifndef INF_H
#define INF_H 1

#include "ptask.h"

// Attempts to find a possible schedule for the passed array of periodic jobs using Iterative Network Flow.
// Stores the schedule, if found, in the passed entries array.
// Returns 0 on success, -1 on failure.
int inf_algorithm(PTaskArrayPtr array, size_t frame_size, size_t frame_count, size_t total_jobs, struct FramePJobEntry entries[frame_count][total_jobs]);

#endif /* INF_H */