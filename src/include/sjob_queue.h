#ifndef SJOB_QUEUE_H
#define SJOB_QUEUE_H 1

#include "sjob.h"

// Dyanimcally-sized FIFO queue of sporadic jobs.
typedef struct SJobQueue *SJobQueuePtr;

SJobQueuePtr SJobQueue_new(void);
void SJobQueue_delete(SJobQueuePtr queue);

void SJobQueue_enqueue(SJobQueuePtr queue, SJobPtr job);
void SJobQueue_dequeue(SJobQueuePtr queue);
SJobPtr SJobQueue_peek(SJobQueuePtr queue);
int SJobQueue_is_empty(SJobQueuePtr queue);



// Dynamically-sized priority queue of sporadic jobs.
// Earlier deadline = higher priority.
typedef struct SJobPriQueue *SJobPriQueuePtr;

SJobPriQueuePtr SJobPriQueue_new(void);
void SJobPriQueue_delete(SJobPriQueuePtr queue);

void SJobPriQueue_enqueue(SJobPriQueuePtr queue, SJobPtr job);
void SJobPriQueue_dequeue(SJobPriQueuePtr queue);
SJobPtr SJobPriQueue_peek(SJobPriQueuePtr queue);
int SJobPriQueue_is_empty(SJobPriQueuePtr queue);

// Tries to enqueue the job after considering all existing jobs in the queue.
// Returns 1 on success, 0 on failure.
int SJobPriQueue_try_enqueue(SJobPriQueuePtr queue, SJobPtr job, double total_slack);


#endif /* SJOB_QUEUE_H */