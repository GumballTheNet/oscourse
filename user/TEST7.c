#include <inc/lib.h>

void
umain(int argc, char **argv)
{
	int mutex = mutex_create();
	mutex_lock(mutex,0,0);
	int son;
	if (!(son = fork())) {
		cprintf("SON 0 BEGIN\n");
		mutex_lock(mutex,0,0);
		ipc_recv(&son, 0, 0);
		cprintf("MY PRIORITY %d\n",thisenv->priority);
		cprintf("SON 0 IS DONE\n");
		mutex_unlock(mutex);
		return;
	}
	sched_setparam(son, 1);
	mutex_unlock(mutex);
	int y[10];
	int i = 1;
	for(; i < 10; ++i) {
		if (!(y[i] = fork())) {
			cprintf("SON %d begin\n",i);
			if (mutex_lock(mutex,0,(10-i)*1000 - 7970 * (i == 2))) {
				cprintf("SON %d FAILED\n",i);	
				if (i == 9)		
					ipc_send(thisenv->env_parent_id,0,0,0);
			}  else {
				for (int k = 0; k < 10000000; ++k){}
				cprintf("SON %d IS DONE\n",i);
			}
			return;
		}
		sched_setparam(y[i], i);

	}
	sched_setparam(thisenv->env_id,10);
	ipc_recv(&y[9],0,0);
	sched_setparam(thisenv->env_id,0);
	ipc_send(son,0,0,0);


}
