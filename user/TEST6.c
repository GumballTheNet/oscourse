#include <inc/lib.h>

void
umain(int argc, char **argv)
{	
	int mut = mutex_create();
	int y[18];
	int i = 0;
	for (; i < 18; ++i) {
		if (!(y[i] = fork())) {
			ipc_recv(&y[i],0,0);
			mutex_lock(mut,0,0);
			ipc_recv(&y[i],0,0);
			cprintf("SON %d WITH PRIORITY %d HERE!\n",i,thisenv->priority);
			return;
		}
		sched_setparam(y[i], i / 2 + 1);
	}
	for (i = 0; i < 18; ++i) {
		ipc_send(y[i],0,0,0);
	}
	ipc_send(y[0],0,0,0);
	for (i = 17; i >= 1; --i) {
		ipc_send(y[i],0,0,0);
	}
	for (i = 0; i < 18; ++i) {
		wait(y[i]);	
	}
}
		


