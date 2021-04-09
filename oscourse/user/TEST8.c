#include <inc/lib.h>

void
umain(int argc, char **argv)
{
	int mut1 = mutex_create(), mut2 = mutex_create(),a,b;
	cprintf("%d\n",mutex_lock(mut2,0,0));
	if (!(a = fork())) {
		if (mutex_lock(mut1,0,0)) {
			cprintf("HMMM\n");
		}
		cprintf("SON 1 0WNS MUTEX 1\n");
		mutex_unlock(mut1);
		cprintf("SON 1 TRIES NEXT\n");
		if (mutex_lock(mut2,0,0)) {
			cprintf("HMMM\n");
		}
		cprintf("SON 1 0WNS MUTEX 2\n");
		mutex_unlock(mut2);
		return;
	}
	if (!(b = fork())) {
		if (mutex_lock(mut2,0,0)) {
			cprintf("HMMM\n");
		}
		cprintf("SON 2 0WNS MUTEX 2\n");
		mutex_unlock(mut2);
		cprintf("SON 2 TRIES NEXT\n");
		if (mutex_lock(mut1,0,0)) {
			cprintf("HMMM\n");
		}
		cprintf("SON 2 0WNS MUTEX 1\n");
		mutex_unlock(mut1);
		return;
	}
	sched_setparam(a, 1);
	sched_setparam(b, 2);
	cprintf("FATHERS PRIORITY %d\n",thisenv->priority);
	mutex_unlock(mut2);
	wait(a);
	wait(b);
}
