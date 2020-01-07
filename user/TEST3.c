#include <inc/lib.h>

void
umain(int argc, char **argv)
{	
	int a = mutex_create();
	int i = 0;
	mutex_lock(a,0,0);
	int y = 0,p[5], k[5], d[5];
	for (; y < 5; ++y){
		if (!(p[y]=fork())) {
			cprintf("1\n");
			if (mutex_lock(a,0,200)) {
				cprintf("SON 1 is tired!\n");
				return;
			}
			cprintf("SON 1 enter\n");
			volatile int j, n = 0;
			for (j = 0; j < 6000000; ++j){n++;};
			mutex_unlock(a);
			cprintf("SON 1 OUT\n");
			return;
		}	
		if (!(k[y] = fork())) {
			cprintf("2\n");
			if (mutex_lock(a,0,200)) {
				cprintf("SON 2 is tired!\n");
				return;
			}
			cprintf("SON 2 enter\n");
			volatile int j, n = 0;
			for (j = 0; j < 6000000; ++j){n++;};
			mutex_unlock(a);
			cprintf("SON 2 OUT\n");
			return;
		}
		if (!(d[y] = fork())) {
			cprintf("3\n");
			if (mutex_lock(a,0,200)) {
				cprintf("SON 3 is tired!\n");
				return;
			}
			cprintf("SON 3 enter\n");
			volatile int j,n = 0;
			for (j = 0; j < 6000000; ++j){n++;};
			mutex_unlock(a);
			cprintf("SON 3 OUT\n");
			return;
		}
	}
	sys_yield();
	for (i = 0; i < 5; ++i) {
		sched_setparam(p[i],0);
	}
	cprintf("FATHERS PRIORITY %d\n",thisenv->priority);
	for (i = 0; i < 5; ++i) {
		sched_setparam(k[i],1);
	}
	cprintf("FATHERS PRIORITY %d\n",thisenv->priority);
	for (i = 0; i < 5; ++i) {
		sched_setparam(d[i],2);
	}
	cprintf("FATHERS PRIORITY %d\n",thisenv->priority);
	mutex_unlock(a);
	for (i = 0; i < 5; ++i) {
		wait(k[i]);
		wait(p[i]);
		wait(d[i]);
	}
}
