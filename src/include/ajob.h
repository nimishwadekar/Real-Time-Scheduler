#ifndef AJOB_H
#define AJOB_H 1

typedef struct AJobQueue *AJobQueuePtr;

AJobQueuePtr AJobQueue_new(void);
void AJobQueue_delete(AJobQueuePtr queue);

void AJobQueue_enqueue(AJobQueuePtr queue, double release_time, double exec_time);
void AJobQueue_dequeue(AJobQueuePtr queue);

// returns job number peeked into.
int AJobQueue_peek(AJobQueuePtr queue, double *release_time, double *exec_time);
int AJobQueue_is_empty(AJobQueuePtr queue);
void AJobQueue_update_exec_time(AJobQueuePtr queue, double remaining_time);

#endif /* AJOB_H */