#ifndef SJOB_QUEUE_H
#define SJOB_QUEUE_H 1

#include "sjob.h"

typedef struct SJobQueue *SJobQueuePtr;

SJobQueuePtr SJobQueue_new(void);
void SJobQueue_delete(SJobQueuePtr queue);

void SJobQueue_enqueue(SJobQueuePtr queue, SJobPtr job);
void SJobQueue_dequeue(SJobQueuePtr queue);
SJobPtr SJobQueue_peek(SJobQueuePtr queue);
int SJobQueue_is_empty(SJobQueuePtr queue);



// Earlier deadline = higher priority.
typedef struct SJobPriQueue *SJobPriQueuePtr;

SJobPriQueuePtr SJobPriQueue_new(void);
void SJobPriQueue_delete(SJobPriQueuePtr queue);

void SJobPriQueue_enqueue(SJobPriQueuePtr queue, SJobPtr job);
void SJobPriQueue_dequeue(SJobPriQueuePtr queue);
SJobPtr SJobPriQueue_peek(SJobPriQueuePtr queue);
int SJobPriQueue_is_empty(SJobPriQueuePtr queue);

// 1 - done, 0 - no
int SJobPriQueue_try_enqueue(SJobPriQueuePtr queue, SJobPtr job, double total_slack);


#endif /* SJOB_QUEUE_H */