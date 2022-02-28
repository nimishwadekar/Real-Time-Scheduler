#ifndef FRAME_H
#define FRAME_H 1

#include "ptask.h"

// Add aperiodic and sporadic jobs to frame.

typedef struct Frame *FramePtr;

FramePtr Frame_new(PTaskArrayPtr ptasks);

void Frame_delete(FramePtr frame);

PTaskArrayPtr Frame_get_ptasks(FramePtr frame);



// Frame Job

typedef struct FrameJob *FrameJobPtr;

// exec time in frame.
double FrameJob_get_execution_time(FrameJobPtr frame_job);

#endif /* FRAME_H */