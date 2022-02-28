#ifndef INF_H
#define INF_H 1

#include "ptask.h"

// 0 on success.
int inf_algorithm(PTaskArrayPtr array, int frame_size, int frame_count, size_t total_jobs, struct FramePJobEntry entries[frame_count][total_jobs]);

#endif /* INF_H */