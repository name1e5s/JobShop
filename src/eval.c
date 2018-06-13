/**
* @file eval.c
* Function to calcuate makespan.
*
* @brief makespan
* @author 于海鑫 - 2017211240
*/
#include "bottle.h"

/**
  Auxiliary struct to calcuate the makespan.
 */
typedef struct JOBMACHINEPAR {
	int			job;
	int			machine;
} job_machine_t;

/** @var magicnum
  The number generated and managed by the God in the computer.
  Every one who changed the name of this feild will be seen as
  an evil and will be cursed by the God.
 */
int magicnum = 0;

/**
  Evaluate the makespan of the given sequence.

  @param seq The sequence of job.
  @return The makespan of the sequence.
 */
int eval(sequence_t* seq) {
	job_machine_t queue[MAXJOB * MAXMACHINE];
	int machine, next_machine, next_job, current_job, makespan;
	int eval_count = 0;
	int count = 0;
	magicnum++;

	// Init...
	for (int i = 0; i < job_size; i++)
		for (int j = 0; j < machine_size; j++)
			job[i].estime[j] = job[i].mhtime[j] = INFINITAS;

	for (int i = 0; i < job_size; i++) {
		machine = job[i].order[0];

		/*
			job[i].prev[machine] == -1 means that this job
			is the first to be processed on the machine...
			IT'S SPECIAL.
		 */
		if (job[i].prev[machine] == -1) {
			count++;
			queue[count].job = i;
			queue[count].machine = machine;
			job[i].estime[machine] = 0;
			eval_count++;
		}
		else
			// ...Or we just need to update its cycle num
			job[i].magic[machine] = magicnum;
	}
	for (int j = 0; j < machine_size; j++) {
		if (seq[j].job[0] == -1)
			for (int i = 0; i < job_size; i++)
				job[i].magic[j] = magicnum;
		else
			job[seq[j].job[0]].magic[j] = magicnum;
	}

	/*
	  Compute estime for every operaton...
	*/
	while (count > 0) {
		current_job = queue[count].job;
		machine = queue[count].machine;
		count--;
		next_job = job[current_job].next[machine];

		if (next_job != -1) {
			if (job[next_job].magic[machine] == magicnum) {
				count++;
				queue[count].job = next_job;
				queue[count].machine = machine;
				if (job[next_job].step[machine] > 0) {
					next_machine = job[next_job].order[job[next_job].step[machine] - 1];
					job[next_job].estime[machine] =
						MAX(job[current_job].estime[machine] +
							job[current_job].process_time[machine],
							job[next_job].estime[next_machine] +
							job[next_job].process_time[next_machine]);
				}
				else
					job[next_job].estime[machine] = job[current_job].estime[machine]
					+ job[current_job].process_time[machine];
				eval_count++;
			}
			else
				job[next_job].magic[machine] = magicnum; // Don't need update them...
		}

		if (job[current_job].step[machine] < machine_size - 1) {
			next_machine = job[current_job].order[job[current_job].step[machine] + 1];
			if (job[current_job].magic[next_machine] == magicnum) {
				count++;
				queue[count].job = current_job;
				queue[count].machine = next_machine;
				next_job = job[current_job].prev[next_machine];
				if (next_job != -1)
					job[current_job].estime[next_machine] =
					MAX(job[next_job].estime[next_machine] +
						job[next_job].process_time[next_machine],
						job[current_job].estime[machine] +
						job[current_job].process_time[machine]);
				else
					job[current_job].estime[next_machine] =
					job[current_job].estime[machine] +
					job[current_job].process_time[machine];
				eval_count++;
			}
			else
				job[current_job].magic[next_machine] = magicnum;
		}
	}

	// When we find that .... It means we find a not feasible solution...We should stop...
	terminate_flag = (job_size * machine_size != eval_count);

	magicnum++;
	count = 0;
	for (int i = 0; i < job_size; i++) {
		machine = job[i].order[machine_size - 1];
		if (job[i].next[machine] == -1) {
			count++;
			queue[count].job = i;
			queue[count].machine = machine;
			job[i].mhtime[machine] = 0;
		}
		else
			job[i].magic[machine] = magicnum;
	}

	for (int j = 0; j < machine_size; j++) {
		if (seq[j].job[job_size - 1] == -1)
			for (int i = 0; i < job_size; i++)
				job[i].magic[j] = magicnum;
		else
			job[seq[j].job[job_size - 1]].magic[j] = magicnum;
	}

	// Compute mhtime for every operaton...
	while (count > 0) {
		current_job = queue[count].job;
		machine = queue[count].machine;
		count--;
		next_job = job[current_job].prev[machine];
		if (next_job != -1) {
			if (job[next_job].magic[machine] == magicnum) {
				count++;
				queue[count].job = next_job;
				queue[count].machine = machine;
				if (job[next_job].step[machine] < machine_size - 1) {
					next_machine = job[next_job].order[job[next_job].
						step[machine] + 1];
					job[next_job].mhtime[machine] =
						MAX(job[next_job].mhtime[next_machine] +
							job[next_job].process_time[next_machine],
							job[current_job].mhtime[machine] +
							job[current_job].process_time[machine]);
				}
				else
					job[next_job].mhtime[machine] = job[current_job].mhtime[machine]
					+ job[current_job].process_time[machine];
			}
			else
				job[next_job].magic[machine] = magicnum;
		}
		if (job[current_job].step[machine] > 0) {
			next_machine = job[current_job].order[job[current_job].step[machine] - 1];
			if (job[current_job].magic[next_machine] == magicnum) {
				count++;
				queue[count].job = current_job;
				queue[count].machine = next_machine;
				next_job = job[current_job].next[next_machine];
				if (next_job != -1)
					job[current_job].mhtime[next_machine] =
					MAX(job[next_job].mhtime[next_machine] +
						job[next_job].process_time[next_machine],
						job[current_job].mhtime[machine] +
						job[current_job].process_time[machine]);
				else
					job[current_job].mhtime[next_machine] = job[current_job].mhtime[machine] +
					job[current_job].process_time[machine];
			}
			else
				job[current_job].magic[next_machine] = magicnum;
		}
	}

	makespan = job[0].estime[job[0].order[machine_size - 1]] +
		job[0].process_time[job[0].order[machine_size - 1]];
	for (int i = 1; i < job_size; i++)
		makespan = MAX(makespan, job[i].estime[job[i].order[machine_size - 1]] +
			job[i].process_time[job[i].order[machine_size - 1]]);
	return makespan;
}
