#include <inc/lib.h>

void
umain(int argc, char **argv)
{	
	sched_setparam(thisenv->env_id, 11);
	int mutex = mutex_create();
	mutex_lock(mutex,0,0);
	int y[10];
	int i = 0;
	for (; i < 10; ++i) {
		if(!(y[i] = fork())) {
			int k = thisenv->env_parent_id;
			if (mutex_lock(mutex,0,0)) {
				cprintf("HMM\n");	
				return;
			}
			ipc_recv(&k,0,0);
			cprintf("SON %d WITH PRIORITY STARTS\n",i);
			ipc_send(k, 0,0,0);
			ipc_recv(&k,0,0);
			cprintf("SON %d WITH PRIORITY IS DONE\n",i);
			return;
		}
		sched_setparam(y[i],i);
	}
	mutex_unlock(mutex);
	for (i = 9; i >= 0; --i) {
		sched_setparam(thisenv->env_id, i);//to communicate
		ipc_send(y[i],0,0,0);
		ipc_recv(&y[i],0,0);
		ipc_send(y[i],0,0,0);
	}
}

