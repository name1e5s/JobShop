/**
* @file bottle.c
* Core algorithms to solve the JSSP.
*
* @brief Core algorithms.
* @author 于海鑫 - 2017211240
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "bottle.h"

#define TRY_COUNT 10

/**
  Store the bottle informnation.
*/
typedef struct BLIST {
	int         machine;					/**< Machine number of this bottle. */
	int         makespan;					/**< Makespan of this bottle. */
	int         order[MAXJOB];				/**< Job order of this bottle. */
} blist_t;

/**
  Machine order type.
*/
typedef struct MACHINEORDER {
	int         size;						/**< Sequenced machine number. */
	int         machines[MAXMACHINE];		/**< Sequenced machine list. */
} mo_t;


/**
  A temporary struct to store a sequence.
*/
typedef struct NEIGHBOR {
	int        next[MAXMACHINE];			/**< You will be either silly or able to understand the name's mean. */
	int        prev[MAXMACHINE];			/**< Same as the previous one. */
} neighbor_t;

/** @var best_makespan
  @brief Store the best makespan value.
 */
int best_makespan = INFINITAS;

static inline void shifting_bottle_neck(sequence_t *seq, mo_t *mo, int *values);
static inline void clear_and_backup_seq(sequence_t *seq, int mach, int *save);
static inline void clear_seq(sequence_t *seq, int machine);
static inline void cp_mo(mo_t *new, mo_t *origin);
static inline void cp_seq(sequence_t *new, sequence_t *origin);
static inline void cp_neighbor(neighbor_t *new);
static inline void re_optimization_phase_1(sequence_t *seq, mo_t *mo, int* makespan);
static inline void set_seq(sequence_t *seq, int mach, int *order);
static inline void save_times(void);
static inline void re_optimization_phase_2(sequence_t *seq, mo_t *mo, int *makespan);
static inline void restore_neighbor(neighbor_t *old);

static inline int critical(int machine, int makespan);
int eval(sequence_t* seq);

/**
  Driver of the Shifting Bottleneck Procedure
 */
void run_bottle_neck(void) {
	sequence_t seq[MAXMACHINE];
	mo_t mo;
	mo.size = 0;

	// Let's start with a trivial machine order...
	for (int i = 0; i < machine_size; i++) {
		mo.machines[i] = i;
	}

	// Initalize the job sequence...
	for (int i = 0; i < machine_size; i++) {
		for (int j = 0; j < job_size; j++) {
			seq[i].job[j] = -1;
			job[j].prev[i] = -1;
			job[j].next[i] = -1;
		}
	}
	eval(seq);
	/** @var times
	  We can change here to have a balance bewteen run time and makespan...
	*/
	int times[10] = { 0x7fffffff,211,11,29,37,1,1,1,1,1 };
	
	// Change its depth depend on its scale...
	if (machine_size > 5)
		shifting_bottle_neck(seq, &mo, &times[1]);
	else if (machine_size >= 15)
		shifting_bottle_neck(seq, &mo, &times[2]);
	else
		shifting_bottle_neck(seq, &mo, times);
}

/**
  The basic idea of the algorithm can be described as follows: It sequences
  the machines one by one successively, taking each time the machine
  identified as a bottleneck among the machine not yet sequenced. Every
  time after a new machine is sequenced, all previously sequenced sequence
  will be locally re-optimized. Bottleneck identification and the local
  re-optimization are both based on solving a one machine scheduling problem,
  which is more easy than the JSSP.
  In this implementation a backtracing trick is introduced to improve the quality
  of the solution, which give us a method to use a slightly more time to
  run the basic shifting bottleneck procedure more times.

  @brief The major implementation of the Shifting Bottleneck Procedure, with a backtracing method.
  @param seq The given sequence list. Will be updated when find a better makespan.
  @param machine_order Machine order.
  @param try_time_set Backtracing depth set.
  @return When the procedure is done. You should find the start time of the solution at
  the "start" field of the struct array job.
 */
static inline void shifting_bottle_neck(sequence_t *seq, mo_t *machine_order, int *try_time_set) {
	// Ooops, we must stop the procedure NOW
	if (terminate_flag)
		return;

	int bot;
	int makespan;
	int bottle_neck = 0;
	int bsize = 0;
	int try_times;
	int try_count = TRY_COUNT;

	mo_t old_machine_order;
	blist_t blist[MAXMACHINE];
	neighbor_t oldneighbor[MAXJOB];
	onemach_times_t one;
	sequence_t oldseq[MAXMACHINE];

	if (machine_size >= 10) {

		try_count = 5;
	} else if (machine_size >= 15) {
		try_count = 3;
	}
	
	if (machine_order->size < try_count)
		try_times = try_time_set[machine_order->size];
	else
		try_times = 1;

	// Initalize all we used...
	for (int next = machine_order->size; next < machine_size; next++) {
		for (int i = 0; i < job_size; i++) {
			one.estime[i] = job[i].estime[machine_order->machines[next]];
			one.process_time[i] = job[i].process_time[machine_order->machines[next]];
			one.mhtime[i] = job[i].mhtime[machine_order->machines[next]];
		}
		blist[bsize].makespan = one_machine(one, blist[bsize].order);
		blist[bsize].machine = next;
		bsize++;
	}
	
	// Ready ?
	cp_mo(&old_machine_order, machine_order);
	cp_seq(oldseq, seq);
	cp_neighbor(oldneighbor);

	// Go !
	while (try_times && bsize) {
		// Identify the bottleneck... The bottleneck must have the biggest makespan...
		for (int i = 1; i < bsize; i++)
			if (blist[i].makespan > blist[bottle_neck].makespan)
				bottle_neck = i;

		bot = machine_order->machines[blist[bottle_neck].machine];
		machine_order->machines[blist[bottle_neck].machine] = machine_order->machines[machine_order->size];
		machine_order->machines[machine_order->size] = bot;
		machine_order->size++;

		// And then do optimization on it...
		set_seq(seq, bot, blist[bottle_neck].order);
		makespan = eval(seq);
		re_optimization_phase_1(seq, machine_order, &makespan);
		re_optimization_phase_2(seq, machine_order, &makespan);

		if (makespan < best_makespan) {
			if (machine_order->size == machine_size) { // Aha! Got a full solution!
				best_makespan = makespan;
#ifdef __TEST
				printf("Ans: %d\n", best_makespan);
#endif
				save_times();
			}
			else {
				shifting_bottle_neck(seq, machine_order, try_time_set); // Handle the unhandled machine recurisively...
			}
		}

		cp_mo(machine_order, &old_machine_order);
		cp_seq(seq, oldseq);
		restore_neighbor(oldneighbor);

		eval(seq);
		bsize--;
		blist[bottle_neck].machine = blist[bsize].machine;
		blist[bottle_neck].makespan = blist[bsize].makespan;

		for (int i = 0; i < job_size; i++)
			blist[bottle_neck].order[i] = blist[bsize].order[i];
		try_times--;
	}
}

/**
  The re-optimization... Phase 1

  @param seq The sequence
  @param machine_order Machine order
  @param makespan Current makespan
 */
static inline void re_optimization_phase_1(sequence_t *seq, mo_t *machine_order, int* makespan) {
	int temp_makespan[MAXMACHINE], old_order[MAXJOB], order[MAXJOB];
	int improvement = 0xada; // Augusta Ada King-Noel, Countess of Lovelace
	onemach_times_t one;

	int oldmakespan = *makespan;
	while (improvement > 0) {
		int newmakespan = oldmakespan;

		for (int i = 0; i < machine_order->size; i++) {
			clear_and_backup_seq(seq, machine_order->machines[i], old_order);
			eval(seq);
			if (terminate_flag) {
				set_seq(seq, machine_order->machines[i], old_order);
				*makespan = eval(seq);
				temp_makespan[i] = 0;
			} else {
				for (int j = 0; j < job_size; j++) {
					one.estime[j] = job[j].estime[machine_order->machines[i]];
					one.process_time[j] = job[j].process_time[machine_order->machines[i]];
					one.mhtime[j] = job[j].mhtime[machine_order->machines[i]];
				}
				temp_makespan[i] = one_machine(one, order);
				set_seq(seq, machine_order->machines[i], order);
				
				newmakespan = eval(seq);
				if (newmakespan < oldmakespan)
					*makespan = newmakespan;
				else {
					set_seq(seq, machine_order->machines[i], old_order);
					newmakespan = oldmakespan;
				}
			}
		}

		int flag = 0;
		for (int i = machine_order->size - 1; i > 0 && flag == 0; i--) {
			flag = 1;
			for (int j = 0; j < i; j++) {
				if (temp_makespan[j] < temp_makespan[j + 1]) {
					flag = 0;

					int t;
					t = temp_makespan[j];
					temp_makespan[j] = temp_makespan[j + 1];
					temp_makespan[j + 1] = t;

					t = machine_order->machines[j];
					machine_order->machines[j] = machine_order->machines[j + 1];
					machine_order->machines[j + 1] = t;
				}
			}
		}
		improvement = oldmakespan - newmakespan;
		oldmakespan = newmakespan;
	}
}

/**
  The re-optimization... Phase 2

  @param seq The sequence
  @param machine_order Machine order
  @param makespan Current makespan
 */
static inline void re_optimization_phase_2(sequence_t *seq, mo_t *machine_order, int *makespan) {
	int oldmakespan;
	sequence_t oldseq[MAXMACHINE];
	mo_t oldmo;
	neighbor_t oldneighbor[MAXJOB];
	onemach_times_t one;
	int order[MAXJOB];
	cp_mo(&oldmo, machine_order);
	cp_neighbor(oldneighbor);
	cp_seq(oldseq, seq);
	oldmakespan = *makespan;
	eval(seq);
	int b = machine_order->size - (int)sqrt((double)machine_order->size);
	for (int i = oldmo.size - 1; i >= 0 && machine_order->size >= b; i--) {
		if (!critical(machine_order->machines[i], *makespan)) {
			clear_seq(seq, machine_order->machines[i]);
			int t = machine_order->machines[i];
			for (int j = i; j < machine_order->size - 1; j++)
				machine_order->machines[j] = machine_order->machines[j + 1];
			machine_order->size--;
			machine_order->machines[machine_order->size] = t;
		}
	}
	*makespan = eval(seq);
	re_optimization_phase_1(seq, machine_order, makespan);
	while (machine_order->size < oldmo.size) {
		if (terminate_flag) {
			cp_mo(machine_order, &oldmo);
			restore_neighbor(oldneighbor);
			cp_seq(seq, oldseq);
			*makespan = eval(seq);
			return;
		}
		for (int i = 0; i < job_size; i++) {
			one.estime[i] = job[i].estime[machine_order->machines[machine_order->size]];
			one.process_time[i] = job[i].process_time[machine_order->machines[machine_order->size]];
			one.mhtime[i] = job[i].mhtime[machine_order->machines[machine_order->size]];
		}
		one_machine(one, order);
		set_seq(seq, machine_order->machines[machine_order->size], order);
		machine_order->size++;
		*makespan = eval(seq);
		re_optimization_phase_1(seq, machine_order, makespan);
	}
	if (*makespan > oldmakespan) {
		cp_mo(machine_order, &oldmo);
		restore_neighbor(oldneighbor);
		cp_seq(seq, oldseq);
		*makespan = eval(seq);
	}
}

/**
  Test wether the machine is the critical machine, which means the end of the
  procedure of this machine is also the end of all the operations.

  @param machine Machine number to be tested.
  @param makespan The given makespan
  @return If the machine is the critical machine, return 1. Else return 0.
 */
static inline int critical(int machine, int makespan) {
	for (int i = 0; i < job_size; i++)
		if (job[i].estime[machine] + job[i].process_time[machine] + job[i].mhtime[machine] == makespan)
			return 1;
	return 0;
}

/**
  Copy the origin machine order to new.
 */
static inline void cp_mo(mo_t *new, mo_t *origin) {
	new->size = origin->size;
	for (int i = 0; i < machine_size; i++)
		new->machines[i] = origin->machines[i];
}

/**
  Store current sequence of machine N in the given address. Then just clear
  the sequence.

  @param seq Sequence to be cleared.
  @param machine Current machine number.
  @param save Address to save the old sequence. A NULL address means the old sequence won't be stored.
*/
static inline void clear_and_backup_seq(sequence_t *seq, int machine, int *save) {
	for (int i = 0; i < job_size; i++) {
		save[i] = seq[machine].job[i];
		seq[machine].job[i] = -1;
		job[i].next[machine] = -1;
		job[i].prev[machine] = -1;
	}
}


/**
  Clear the sequence.

  @param seq Sequence to be cleared.
  @param machine Current machine number.
*/
static inline void clear_seq(sequence_t *seq, int machine) {
	for (int i = 0; i < job_size; i++) {
		seq[machine].job[i] = -1;
		job[i].next[machine] = -1;
		job[i].prev[machine] = -1;
	}
}


/**
  Copy the origin sequence order to new.
*/
static inline void cp_seq(sequence_t *new, sequence_t *origin) {
#pragma parallel
	for (int i = 0; i < machine_size; i++)
		for (int j = 0; j < job_size; j++)
			new[i].job[j] = origin[i].job[j];
}

/**
  Set sequence by the given order.

  @param seq Sequence to be set.
  @param machine The machine which the sequence relies on.
  @param order The given order.
*/
static inline void set_seq(sequence_t *seq, int machine, int *order) {
#pragma parallel
	for (int i = 0; i < job_size - 1; i++) {
		seq[machine].job[i] = order[i];
		job[order[i]].next[machine] = order[i + 1];
		job[order[i + 1]].prev[machine] = order[i];
	}
	seq[machine].job[job_size - 1] = order[job_size - 1];
	job[order[job_size - 1]].next[machine] = -1;
	job[order[0]].prev[machine] = -1;
}

/**
  Store neighbor to a neighbor_t varible.
*/
static inline void cp_neighbor(neighbor_t *new) {
	for (int i = 0; i < job_size; i++)
		for (int j = 0; j < machine_size; j++) {
			new[i].next[j] = job[i].next[j];
			new[i].prev[j] = job[i].prev[j];
		}
}

/**
  Load neighbor from a neighbor_t varible.
*/
static inline void restore_neighbor(neighbor_t *old) {
	for (int i = 0; i < job_size; i++) {
		for (int j = 0; j < machine_size; j++) {
			job[i].next[j] = old[i].next[j];
			job[i].prev[j] = old[i].prev[j];
		}
	}
}

/**
  Save current start time of each operation.
 */
static inline void save_times(void) {
	for (int i = 0; i < job_size; i++)
		for (int j = 0; j < machine_size; j++)
			job[i].start[j] = job[i].estime[j];
}