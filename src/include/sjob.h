#ifndef SJOB_H
#define SJOB_H 1

// A sporadic job.
typedef struct SJob *SJobPtr;

SJobPtr SJob_new(int job_no, double arrival, double exec_time, double abs_deadline);
void SJob_delete(SJobPtr job);

int SJob_number(SJobPtr job);
double SJob_arrival(SJobPtr job);
double SJob_exec_time(SJobPtr job);
void SJob_set_exec_time(SJobPtr job, double exec_time);
double SJob_abs_deadline(SJobPtr job);
double SJob_slack(SJobPtr job);
void SJob_set_slack(SJobPtr job, double slack);

#endif /* SJOB_H */