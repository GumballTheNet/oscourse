#include <inc/lib.h>

int mutex_lock(int mut_id, int try, int time) {
	if (thisenv->holding != -1) {
		return -1;
	}
	sys_mutex_lock(mut_id, try, time);
	if (thisenv->holding == -1) {
		if (try) {
			return -1;
		}
		sys_yield();
		return sys_check_after(mut_id, time > 0);
	} else {
		return 0;
	}
}

int mutex_unlock(int mut_id) {	
	return sys_mutex_unlock(mut_id);
}

int mutex_create() {
	return sys_mutex_create();
}

int mutex_delete(int mut_id) {
	return sys_mutex_delete(mut_id);
}
