/**
 * @file io.c
 * Functions to handle input and output.
 *
 * @brief IO
 * @author 田静悦 - 2017211259
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bottle.h"

/**
  A temporary struct for converting the internal representation
  of the solution to the format required by those soreheads.
 */
typedef struct PAIR_ASSISTANT_TYPE {
	int start_time;		/**< Start time of this node. */
	int job_num;		/**< Serial number of the job of this node. */
	int mach_num;		/**< Serial number of the machine of this node. */
	int proc_time;		/**< Process time (a.k.a duration) of this node.*/
	int step;			/**< Serial number of the order of this node in the job. */
} pair_ass_t;

extern int best_makespan;

static inline void getprob(void);
static inline void write_file(char *file_name, int ans, pair_ass_t* pairs, float times);
#ifdef __TEST
static inline _Bool test_output( pair_ass_t *pair);
#endif

/**
  Function to compare machine number of two pairs for qsort.

  @param a The first pair.
  @param b The second pair.
  @return If machine number of a is lesser than b, then return a positive
  value, else return a non-positive value.
 */
int machine_sort_cmp(const void *a, const void *b) {
	return ((pair_ass_t *)a)->mach_num - ((pair_ass_t *)b)->mach_num;
}

/**
  Function to compare start time of two pairs for qsort.

  @param a The first pair.
  @param b The second pair.
  @return If start time of a is lesser than b, then return a positive
  value, else return a non-positive value.
*/
int starttime_sort_cmp(const void *a, const void *b) {
	return ((pair_ass_t *)a)->start_time - ((pair_ass_t *)b)->start_time;
}

/**
  Convert internal solution representation structure to
  the format required by those nitpickers and print it.

  @param filename Instance file path
 */
void prestissimo(void) {
	clock_t t = clock();
	pair_ass_t pairs[MAXJOB*MAXMACHINE];
	getprob();
	run_bottle_neck();
	for (int i = 0; i < job_size; i++) {
		for (int j = 0; j < machine_size; j++) {
			pairs[i * machine_size + j].start_time = job[i].start[j];
			pairs[i * machine_size + j].job_num = i;
			pairs[i * machine_size + j].mach_num = j;
			pairs[i * machine_size + j].proc_time = job[i].process_time[j];
			pairs[i * machine_size + j].step = job[i].step[j];
		}
	}
	qsort(pairs, job_size * machine_size, sizeof(pairs[0]), machine_sort_cmp);
	for (int j = 0; j < machine_size; j++) {
		qsort(pairs + job_size * j, job_size, sizeof(pairs[0]), starttime_sort_cmp);
	}
	float used_time = (clock() - t) * 1.0f / CLOCKS_PER_SEC;
#ifdef __TEST
	if (!test_output(pairs))
		printf("Result is invalid!\n");
#endif
	for (int i = 0; i < machine_size; i++) {
		printf("M%d", i);
		for (int j = 0; j < job_size; j++) {
			pair_ass_t *t = pairs + i * job_size + j;
			printf(" (%d,%d-%d,%d)", t->start_time, t->job_num, t->step, t->start_time + t->proc_time);
		}
		printf("\n");
	}
	printf("Time Used: %.3fs\nEnd Time: %d\n",used_time,best_makespan);
	write_file("32output.txt",best_makespan,pairs,used_time);
	return;
}

/**
  Read instance file.

  @param file_name Instance file path
 */
static inline void getprob(void) {
	int process_time, machine;
	scanf("%d%d", &job_size, &machine_size);
	for (int i = 0; i < job_size; i++)
		for (int j = 0; j < machine_size; j++) {
			scanf("%d%d", &machine, &process_time);
			job[i].order[j] = machine;
			job[i].process_time[machine] = process_time;
			job[i].step[machine] = j;
		}
}

/**
  Print result to file...

  @param file_name Instance file path
  @param pairs Pair to be printed...
 */
static inline void write_file(char *file_name, int ans,pair_ass_t* pairs,float times) {
	FILE *out;
	if ((out = fopen(file_name, "w")) == NULL) {
		fprintf(stderr, "Unable to open %s\n", file_name);
		exit(1);
	}

	for (int i = 0; i < machine_size; i++) {
		fprintf(out,"M%d", i);
		for (int j = 0; j < job_size; j++) {
			pair_ass_t *t = pairs + i * job_size + j;
			fprintf(out," (%d,%d-%d,%d)", t->start_time, t->job_num, t->step, t->start_time + t->proc_time);
		}
		fprintf(out,"\n");
	}
	fprintf(out,"Time Used: %.3fs\nEnd Time: %d\n", times, ans);

}

#ifdef __TEST
/********************************************/
/*                TEST FIELD                */
/********************************************/

/**
  Test if our output is valid...

  @param job Instance
  @param pairs Pair to be checked...
*/
static inline _Bool test_output(pair_ass_t *pair) {
	_Bool flag = 1;
	for (int i = 0; i < job_size * machine_size;i++) {
		if(job[pair[i].job_num].order[pair[i].step] != pair[i].mach_num) {
			printf("Result unmatch at job %d machine %d !\n", pair[i].job_num, pair[i].mach_num);
			flag = 0;
		}
	}
	return flag;
}
#endif
