# Real-Time Scheduler

This is an implementation of a real-time scheduler in C. It is an application of the information learned in the course Real-Time Systems. The schedule schedules periodic tasks, aperiodic jobs and sporadic jobs. 
Periodic tasks are scheduled using the Iterative Network Flow algorithm. 
Aperiodic jobs implement slack stealing to minimise their response time. 
Sporadic jobs are scheduled following the Earliest Deadline First algorithm.

The program first reads the periodic tasks from the periodic task file, and schedules them using the INF algorithm. It uses the Edmonds-Karp algorithm to find maximum flow. It outputs the obtained cyclic schedule for periodic job slices.

It then reads the aperiodic and sporadic job files, and in conjunction with the periodic job schedule, schedules all jobs available by simulating run time using a time variable. Job arrival interrupts are simulated by allocating separate queues for jobs not yet arrived into the system. These queues are then polled at certain intervals during scheduling to check if a job can be taken from them. The scheduler ends run time when no more aperiodic or sporadic jobs can arrive into the system.

The scheduler regularly outputs the jobs chosen to run to a data structure. This data structure is then finally output to the screen and, optionally, a file.


## Compilation
Run the following command to compile the scheduler. The binary after compilation is located in ```bin/```.
```
> make
```
#### Note: 
Use "make clean" to delete all object and binary files.

## Execution
Run the following command to execute the scheduler.
```
> ./bin/scheduler <periodic tasks file> <aperiodic jobs file> <sporadic jobs file>
```
OR
```
> ./bin/scheduler <periodic tasks file> <aperiodic jobs file> <sporadic jobs file> <optional file>
```
The optional 4th argument is used to direct the final scheduled output to a file. All arguments are file names. The job files need to be in the following format:

File data formats:
- Periodic tasks file:
  Line 1: Number of tasks.
  n (number of tasks) lines after that.
  Each of those lines: ```phase (int) <space> period (int) <space> execution time (double) <space> relative deadline (int)```

- Aperiodic jobs file:
  Line 1: Number of jobs.
  n (number of tasks) lines after that.
  Each of those lines: ```arrival time (double) <space> execution time (double)```

- Sporadic jobs file:
  Line 1: Number of jobs.
  n (number of tasks) lines after that.
  Each of those lines: ```arrival time (double) <space> execution time (double) <space> absolute deadline (double)```

Sample files have been provided in the repository.

#### Note: 
The program by default implements randomized execution times for all jobs. In case one wants all jobs to use their worst-case execution times, simply open the file ```src/include/common.h``` and comment out the line ```#define RAND_EXEC_TIME 1```. Then run "make clean" before re-compiling the program. Do the reverse to enable it again. Do not forget to run "make clean" every time one does this.
