#ifndef SCHEDULER_H
#define SCHEDULER_H 1

typedef struct Scheduler *SchedulerPtr;

SchedulerPtr Scheduler_new(void);
void Scheduler_delete(SchedulerPtr scheduler);

// Non-zero return value = error.
int schedule_periodic_tasks(const char *task_file_name, const char *sched_file_name);

int Scheduler_load_periodic_schedule(SchedulerPtr scheduler, const char *file_name);


void Scheduler_cyclic_executive(SchedulerPtr scheduler);

#endif /* SCHEDULER_H */