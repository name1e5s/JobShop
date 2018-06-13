#include "bottle.h"

/** @var job
  Data representation of all the jobs. All operations runs on this varible.
*/
job_t job[MAXJOB];

/** @var job_size
  Job number in this instance.
*/
int job_size;

/** @var machine_size
  Machine number in this instance.
*/
int machine_size;

/** @var terminate_flag
  Should we stop???
*/
int terminate_flag;
