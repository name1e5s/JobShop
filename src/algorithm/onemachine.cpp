/**
* @file onemachine.c
* One-machine Sequencing Algorithm from Jacques Carlier
*
* @brief One-machine sequencing
* @author TJenica
*/

#include <stdio.h>
#include <string.h>
#include <bottle.h>

/** @def ONEMACH_BBNODES
  Nodes number of the branch and bound tree to solve the one machine
  sequencing problem.
*/
#define ONEMACH_BBNODES 300
/**
  Info of node of a branch and bound tree.
 */
typedef struct ONEMACHINE_BRANCH_AND_BOUND_ASSISTANT {
	int        active;				/**< Wether this node is active */
	int        bound;				/**< See "JOBTYPE" for more info. */
} onemach_bb_ass_t;

static inline void cp_onemach_time(onemach_times_t *mew, onemach_times_t *origin);
static inline void mwr_schedule(onemach_times_t one, int *order);

static inline int lower_bound(onemach_times_t one, int *jset, int jset_size);
static inline int make_span(onemach_times_t one, int *order, int *jset, int *jset_size, int *cjob, int *pjob, int *make);
static inline int make_value(onemach_times_t one, int *order);

/**
  The one-machine sequencing algorithm from
  "The one-machine sequencing problem" by Jacques Carlier.

  @param one Representation of the machine.
  @param bestorder Best job order
  @return makespan
*/
int one_machine(onemach_times_t one, int *bestorder) {
	onemach_times_t current_one, bbnodes[ONEMACH_BBNODES];
	onemach_bb_ass_t bbass[ONEMACH_BBNODES];
	int order[MAXJOB];
	int make, lbound, lbound1,lbound2, bestbound, temp_time, temp_process_time;
	int active_cjob, active_t_pjob;
	int job_set[MAXJOB], job_set_size;
	int critical_job_order, terminate_job_order, last, iscritical;

	// Phase 1: Initialize all parameters
	for (int i = 0; i < job_size; i++)
		bestorder[i] = 0;

	for (int i = 0; i < ONEMACH_BBNODES; i++)
		bbass[i].active = bbass[i].bound = 0;

	int onemake = INFINITAS;
	last = 0;
	bbass[0].active = 1;

	cp_onemach_time(&(bbnodes[0]), &one);

	for (;;) {
		bestbound = onemake;
		int temp_count = 0;
		for (int i = 0; i <= last; i++)
			if (bbass[i].active)
				if (bbass[i].bound < bestbound) {
					bestbound = bbass[i].bound;
					temp_count = i;
				}
		if (bestbound == onemake)
			return onemake;

		cp_onemach_time(&current_one, &(bbnodes[temp_count]));

		bbass[temp_count].active = 0;
		mwr_schedule(current_one, order);
		iscritical = make_span(current_one, order, job_set, &job_set_size, &critical_job_order,
			&terminate_job_order, &make);
		if (make < onemake) {
			onemake = make;
			for (int i = 0; i < job_size; i++)
				bestorder[i] = order[i];
		}
		if (iscritical == 0)
			continue;

		lbound1 = lower_bound(current_one, job_set, job_set_size);

		job_set[job_set_size] = order[critical_job_order];
		job_set_size++;

		lbound2 = lower_bound(current_one, job_set, job_set_size);

		lbound = MAX(lbound1, lbound2);
		job_set_size--;
		if (lbound >= onemake)
			continue;

		active_cjob = order[critical_job_order];
		active_t_pjob = order[terminate_job_order];

		for (int i = critical_job_order; i <= terminate_job_order - 1; i++)
			order[i] = order[i + 1];

		order[terminate_job_order] = active_cjob;
		make = make_value(current_one, order);
		if (make < onemake) {
			for (int i = 0; i < job_size; i++)
				bestorder[i] = order[i];
			onemake = make;
			if (onemake == bestbound)
				return onemake;
			if (lbound >= onemake)
				continue;
		}

		bbass[temp_count].active = 1;
		bbass[temp_count].bound = MAX(lbound, bestbound);

		cp_onemach_time(&(bbnodes[temp_count]), &current_one);

		temp_time = current_one.mhtime[active_t_pjob];
		for (int i = 0; i < job_set_size; i++)
			temp_time += current_one.process_time[job_set[i]];
		if (temp_time > current_one.mhtime[active_cjob])
			bbnodes[temp_count].mhtime[active_cjob] = temp_time;

		temp_count = 0;
		while (bbass[temp_count].active)
			temp_count++;

		bbass[temp_count].active = 1;
		bbass[temp_count].bound = MAX(lbound, bestbound);

		cp_onemach_time(&(bbnodes[temp_count]), &current_one);

		temp_time = current_one.estime[job_set[0]];
		temp_process_time = current_one.process_time[job_set[0]];

		for (int i = 1; i < job_set_size; i++) {
			temp_time = MAX(current_one.estime[job_set[i]], temp_time);
			temp_process_time += current_one.process_time[job_set[i]];
		}

		if (current_one.estime[active_cjob] < temp_time + temp_process_time)
			bbnodes[temp_count].estime[active_cjob] = temp_time + temp_process_time;
		if (last < temp_count)
			last = temp_count;
	}
}

/**
  Copy origin onemach_times struct to mew
  By practice, change from memcpy to just write what we want to do
  is very important...
 */
static inline void cp_onemach_time(onemach_times_t *mew, onemach_times_t *origin) {
	for (int i = 0; i < job_size; i++) {
        mew->estime[i] = origin->estime[i];
        mew->process_time[i] = origin->process_time[i];
        mew->mhtime[i] = origin->mhtime[i];
	}
}

/**
  Algorithm to find the most work remaining schedule by Schrage
 */
static inline void mwr_schedule(onemach_times_t one, int *order) {
	int ready[MAXJOB], scheduled[MAXJOB];
	int next = 0;
	int scheduled_num = 0;

	for (int i = 0; i < job_size; i++)
		ready[i] = scheduled[i] = 0;
	int temp_estime = one.estime[0];

	for (int i = 1; i < job_size; i++)
		if (one.estime[i] < temp_estime)
			temp_estime = one.estime[i];

	while (scheduled_num < job_size) {
		for (int i = 0; i < job_size; i++)
			if (one.estime[i] <= temp_estime)
				ready[i] = 1;

		int temp_mhtime = -1;
		for (int i = 0; i < job_size; i++) {
			if (ready[i] && !scheduled[i])
				if (temp_mhtime < one.mhtime[i]) {
					temp_mhtime = one.mhtime[i];
					next = i;
				}
		}

		order[scheduled_num] = next;
		scheduled_num++;

		scheduled[next] = 1;
		if (scheduled_num < job_size) {
			temp_estime += one.process_time[next];
			temp_mhtime = INFINITAS;
			for (int i = 0; i < job_size; i++) {
				if (!scheduled[i])
					if (temp_mhtime > one.estime[i])
						temp_mhtime = one.estime[i];
			}

			if (temp_estime < temp_mhtime)
				temp_estime = temp_mhtime;
		}
	}
}

/**
  Compute the makespan of the given job order.

  @param one  Representation of the machine.
  @order order The given job order.
 */
static inline int make_value(onemach_times_t one, int *order) {

	int head = one.estime[order[0]];
	int value = one.estime[order[0]] + one.process_time[order[0]] + one.mhtime[order[0]];

	for (int i = 1; i < job_size; i++) {
		head = MAX(head + one.process_time[order[i - 1]],
			one.estime[order[i]]);
		value = MAX(value, head + one.process_time[order[i]] +
			one.mhtime[order[i]]);
	}
	return value;
}

/**
  Find the lower bound of the given machine on the given job order.

  @param one The representation of the given machine.
  @param job_set The set of job.
  @param job_set_size The size of job_set
  @return Lowerbound of the machine. Which is just the sum of minimum estime
  and minimum mhtime and the sum of all the process time.
 */
static inline int lower_bound(onemach_times_t one, int *job_set, int job_set_size) {
	if (job_set_size == 0)
		return 0;

	int temp = job_set[0];
	int total_process_time = one.process_time[temp];
	int min_estime = one.estime[temp];
	int min_mhtime = one.mhtime[temp];

	for (int i = 1; i < job_set_size; i++) {
		temp = job_set[i];
		if (one.estime[temp] < min_estime)
			min_estime = one.estime[temp];
		if (one.mhtime[temp] < min_mhtime)
			min_mhtime = one.mhtime[temp];
		total_process_time += one.process_time[temp];
	}
	return (total_process_time + min_estime + min_mhtime);
}

/**
   Test if the job order is feasible and compute the make_span.

   @param one The representation of the given machine.
   @param order The given job order.
   @param job_set The set of job on the machine.
   @param job_set_size The size of job_set.
   @param critical_job_order
   @param terminate_job_order
   @param make The make_span.
   @return If the order is OK return 1,else return 0.
 */
static inline int make_span(onemach_times_t one, int *order, int *job_set, int *job_set_size, int *critical_job_order, int *terminate_job_order, int *make) {
	int end_time[MAXJOB];
	int temp_job_order;

	end_time[0] = one.estime[order[0]] + one.process_time[order[0]];
	*make = end_time[0] + one.mhtime[order[0]];
	*terminate_job_order = 0;

	for (int i = 1; i < job_size; i++) {
		end_time[i] = MAX(end_time[i - 1], one.estime[order[i]]) +
			one.process_time[order[i]];
		if (*make <= end_time[i] + one.mhtime[order[i]]) {
			*make = end_time[i] + one.mhtime[order[i]];
			*terminate_job_order = i;
		}
	}

	for (temp_job_order = *terminate_job_order - 1; temp_job_order >= 0; temp_job_order--) {
		if (one.estime[order[temp_job_order + 1]] > end_time[temp_job_order]) {
			return 0;
		}
		if (one.mhtime[order[temp_job_order]] < one.mhtime[order[*terminate_job_order]])
			break;
	}

	if (temp_job_order < 0)
		return 0;

	*critical_job_order = temp_job_order;
	*job_set_size = 0;
	for (int i = *critical_job_order + 1; i <= *terminate_job_order; i++) {
		job_set[*job_set_size] = order[i];
		(*job_set_size)++;
	}
	return 1;
}
