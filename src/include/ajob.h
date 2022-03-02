#ifndef AJOB_H
#define AJOB_H 1

// Dynamically-sized FIFO queue of aperiodic jobs.
typedef struct AJobQueue *AJobQueuePtr;

AJobQueuePtr AJobQueue_new(void);
void AJobQueue_delete(AJobQueuePtr queue);

void AJobQueue_enqueue(AJobQueuePtr queue, double release_time, double exec_time);
void AJobQueue_dequeue(AJobQueuePtr queue);

// Peeks into the front of the queue and stores the release time and the execution time of the job in the front in the pointers passed.
// Returns the job number.
int AJobQueue_peek(AJobQueuePtr queue, double *release_time, double *exec_time);

int AJobQueue_is_empty(AJobQueuePtr queue);

// Updates the execution time of the job at the front of the queue with the passed value.
void AJobQueue_update_exec_time(AJobQueuePtr queue, double remaining_time);

#endif /* AJOB_H */