#include <bits/types/FILE.h>
#ifndef SCHEDULER_H
#define SCHEDULER_H 1

#include <stddef.h>

enum JobType
{
	JOB_TYPE_PERIODIC,
	JOB_TYPE_APERIODIC,
	JOB_TYPE_SPORADIC,
	JOB_TYPE_IDLE
};

// The scheduler.
typedef struct Scheduler *SchedulerPtr;

// Schedule produced by the scheduler.
typedef struct Schedule *SchedulePtr;

SchedulerPtr Scheduler_new(void);
void Scheduler_delete(SchedulerPtr scheduler);

// Reads list of tasks from file task_file_name, schedules all of them, and writes the result to file sched_file_name.
// Non-zero return value = error.
int schedule_periodic_tasks(const char *task_file_name, const char *sched_file_name);

int Scheduler_load_periodic_schedule(SchedulerPtr scheduler, const char *file_name);
int Scheduler_load_aperiodic_schedule(SchedulerPtr scheduler, const char *file_name);
int Scheduler_load_sporadic_schedule(SchedulerPtr scheduler, const char *file_name);

// Executes the schedule as if at runtime.
void Scheduler_cyclic_executive(SchedulerPtr scheduler, SchedulePtr schedule);



SchedulePtr Schedule_new(void);
void Schedule_delete(SchedulePtr schedule);

size_t Schedule_size(SchedulePtr schedule);
void Schedule_set_frame_size(SchedulePtr schedule, size_t frame_size);
void Schedule_add_entry(SchedulePtr schedule, double slice_time, enum JobType job_type, int job, int task_if_periodic);
void Schedule_get_entry(SchedulePtr schedule, size_t index, double *slice_time, enum JobType *job_type, int *job, int *task_if_periodic);

void Schedule_fprint(SchedulePtr schedule, FILE *stream);

const char* JobType_str(enum JobType job_type);

#endif /* SCHEDULER_H */