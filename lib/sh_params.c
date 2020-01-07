#include <inc/lib.h>

const int sched_rr_get_interval() {
	return QUANTUM;
}

const int sched_get_priority_min() {
	return 0;
}

const int sched_get_priority_max() {
	return PRIOR_COUNT;
}
