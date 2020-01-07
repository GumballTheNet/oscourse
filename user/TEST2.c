#include <inc/lib.h>

void umain(int argc, char **argv) {
	int y[5];
	int i = 0;
	sched_setparam(thisenv->env_id, 6);
	for (i = 0; i < 5; ++i) {
		if (!(y[i] = fork())) {
			int k = thisenv->env_parent_id;
			ipc_send(k,0,0,0);
			cprintf("Now in son with priority %d\n",thisenv->priority);
			volatile int j = 0;		
			for (; j < 100000000; ++j){}
			cprintf("Leave son %d\n",i);
			return;
		}
	}
	sched_setparam(y[0],1);
	sched_setparam(y[1],1);
	sched_setparam(y[2],2);
	sched_setparam(y[3],2);
	sched_setparam(y[4],3);
	ipc_recv(&y[0],0,0);
	ipc_recv(&y[1],0,0);
	ipc_recv(&y[2],0,0);
	ipc_recv(&y[3],0,0);
	ipc_recv(&y[4],0,0);
	return;
}
