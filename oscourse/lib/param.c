
#include <inc/lib.h>
int sched_setparam(size_t env_id, unsigned priority)
{ 
	return  sys_sched_setparam(env_id, priority);
}

