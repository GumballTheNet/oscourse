#include <inc/lib.h>

void
umain(int argc, char **argv)
{	
	int a = mutex_create();
	mutex_lock(a,0,0);
	int y = 0;
	int p[5],k[5],d[5];
	for (; y < 5; ++y){
		if (!(p[y]=fork())) {cprintf("1\n");
			if (mutex_lock(a,0,0)) {
				cprintf("HMM!\n");
				return;
			}
			cprintf("SON 1 enter\n");
			volatile int j, n = 0;
			for (j = 0; j < 15000000; ++j){n++;};
			mutex_unlock(a);
			cprintf("SON 1 OUT\n");
			return;
		}	
		sched_setparam(p[y], 1);
		if (!(k[y] = fork())) {cprintf("2\n");
			if (mutex_lock(a,0,0)) {
				cprintf("HMM!\n");
				return;
			}
			cprintf("SON 2 enter\n");
			volatile int j, n = 0;
			for (j = 0; j < 15000000; ++j){n++;};
			mutex_unlock(a);
			cprintf("SON 2 OUT\n");
			return;
		}
		if (!(d[y] = fork())) {cprintf("3\n");
			if (mutex_lock(a,0,0)) {
				cprintf("HMM!\n");
				return;
			}
			cprintf("SON 3 enter\n");
			volatile int j,n = 0;
			for (j = 0; j < 15000000; ++j){n++;};
			mutex_unlock(a);
			cprintf("SON 3 OUT\n");
			return;
		}
		sched_setparam(k[y],2);
		sched_setparam(d[y],3);

	}
	mutex_unlock(a);
	for (y = 0; y < 5; ++y) {
		wait(p[y]);
		wait(k[y]);
		wait(d[y]);
	}
}
