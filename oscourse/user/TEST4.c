#include <inc/lib.h>

void
umain(int argc, char **argv)
{	
	int i = 0;
	cprintf("%d\n",sched_rr_get_interval());
	cprintf("%d\n",sched_get_priority_min());
	cprintf("%d\n",sched_get_priority_max());
	for (; i < 10; ++i) {
		if (!fork()) {
			cprintf("NOW IN SON %d\n",i);
			while (1) {}
			return;
		}
	}
}
