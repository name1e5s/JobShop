/**
 * @file bottle.h
 * A Simple Old-fashion Implementation Of The
 * Well-known Shifting Bottleneck Procedure For
 * Job Shop Scheduling Problem(JSSP).The codes
 * are based on "The Shifting Bottleneck Procedure for Job Shop Scheduling"
 * by J. Adams et al.
 *
 * @brief Header file for the whole project.
 * @author Name1e5s
 */

#ifndef BOTTLE_H_INCLUDED
#define BOTTLE_H_INCLUDED
#ifdef _MSC_VER
#pragma comment(linker, "/STACK:10240000000,10240000000")
#endif
 /** @def MAXJOB
  The most jobs this program can handle.
 */
#define MAXJOB 30

/** @def MAXMACHINE
 The most machines this program can handle.
 */
#define MAXMACHINE 30

/** @def INFINITAS
  A integer that can be seen as infinity -- should be bigger than the biggest makespan
  of all the instances. Hence, 0x7fffffff (a.k.a INT_MAX) is a good
  choice
 */
#define INFINITAS 0x7fffffff

/** @def MAX(a,b)
  A regular macro that returns the bigger value bewteen a and b.
 */
#define MAX(a,b) ((a) > (b) ? (a) : (b))

 /**
   Data representation for a job.
 */
typedef struct JOB {
	int         estime[MAXMACHINE];			/**< Earlist starting time of this job on each machine. Which is simply the sum of this job's processing times on the machine before [order[machine]] in this jobs prescribed ordering. */
	int         mhtime[MAXMACHINE];			/**< Minimum halting time of this job after [machine num]. Which is simply the sum of this job's processing times on the machine after [order[machine]] in this jobs prescribed ordering.*/
	int         magic[MAXMACHINE];			/**< The number generated and managed by the God in the computer. Every one who changed the name of this feild will be seen as an evil and will be cursed by the God. */
	int         order[MAXMACHINE];			/**< Required machine order for the job. */
	int         process_time[MAXMACHINE];	/**< Process time of each machine. */
	int         step[MAXMACHINE];			/**< Solution step indexed by machine. */
	int         next[MAXMACHINE];			/**< Next job on machine [i]. */
	int         prev[MAXMACHINE];			/**< Previous job blah blah. */
	int         start[MAXMACHINE];			/**< Start time of this job on each machine. */
} job_t;

/**
  Job sequences on a machine.
 */
typedef struct SEQUENCE {
	int        job[MAXJOB];				/**< Job sequences on a machine. */
} sequence_t;

/**
  Store the time info for every job runs on the same machine.
 */
typedef struct ONEMACHINestime {
	int        estime[MAXJOB];				/**< See "JOBTYPE" for more info. */
	int        mhtime[MAXJOB];				/**< See "JOBTYPE" for more info. */
	int        process_time[MAXJOB];		/**< See "JOBTYPE" for more info. */
} onemach_times_t;

// Some useful vars
extern job_t job[MAXJOB];
extern int job_size, machine_size;
extern int terminate_flag;

// Functions
void prestissimo(void);
void run_bottle_neck(void);

int one_machine(onemach_times_t one, int *bestorder);
#endif // BOTTLE_H_INCLUDED
