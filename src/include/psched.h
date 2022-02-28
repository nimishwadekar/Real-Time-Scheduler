#ifndef PSCHED_H
#define PSCHED_H 1

// Non-zero return value = error.
int schedule_periodic_tasks(const char *task_file_name, const char *sched_file_name);

#endif /* PSCHED_H */