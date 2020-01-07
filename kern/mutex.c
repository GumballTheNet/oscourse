#include <inc/types.h>
#include <inc/assert.h>
#include <inc/x86.h>
#include <inc/memlayout.h>
#include <inc/string.h>
#include <kern/env.h>
#include <kern/cpu.h>
#include <kern/spinlock.h>
#include <kern/kdebug.h>
#include <kern/mutex.h>
#include <kern/sched.h>

struct mutex mut_array[NENV] = {0};

size_t code(int i) {
	return i + 1000;
}

int decode(size_t i) {
	int res = i - 1000;
	if (res < NENV && res >= 0) {
		return res;
	} else {
		return -1;
	}
}

int extract_from_waiting(struct mutex *mut, struct Env *env) {
	struct Env *tmp = mut->waiting_envs;
	if (tmp == env) {
		mut->waiting_envs = mut->waiting_envs->wait_next;		
	} else {
		while (tmp && tmp->wait_next != env) {
			tmp = tmp->wait_next;
		}
		if (!tmp) {
			return -1;
		}
		tmp->wait_next = env->wait_next;

	}
	env->wait_next = NULL;
	env->holding = -1;
	set_status(env, ENV_RUNNABLE);
	return 0;
}

int add_to_waiting(struct mutex *mut, int time, int status) {
	curenv->wait_next = mut->waiting_envs;
	mut->waiting_envs = curenv;
	curenv->wait_time = time;
	set_status(curenv, status);
	return 0;
}

int wake_one(struct mutex *mut) {
	struct Env *tmp = mut->waiting_envs;
	if (tmp == NULL) {
		return -1;
	}
	struct Env *to_wake = tmp;
	while (tmp) {
		if (tmp->priority > to_wake->priority) {
			to_wake = tmp;
		}
		tmp = tmp->wait_next;
	}
	extract_from_waiting(mut, to_wake);
	mut->owner = to_wake;
	mut->owner_start_priority = mut->owner->priority;
	to_wake->holding = code((unsigned)(mut - mut_array));
	return 0;
}

int wake_all(struct mutex *mut) {
	struct Env *tmp = mut->waiting_envs;
	while (tmp) {
		extract_from_waiting(mut,tmp);
		tmp = mut->waiting_envs;
	}
	return 0;
}



void s_mutex_lock(int mut_id, int try, int time) {
	int id = decode(mut_id);
	if (id < 0) {
		return;
	}
	struct mutex *mut = &mut_array[id];
	if (!mut->allocated) {
		return;
	}
	if (!(xchg(&mut->locked, 1))) {
		mut->owner = curenv;
		curenv->holding = mut_id;
		mut->owner_start_priority = mut->owner->priority;
		return;
	} else if (try) {
		return;
	} else {
		curenv->holding = mut_id;
		spin_lock(mut->prior_lock);
		if (mut->locked && mut->owner->priority < curenv->priority) {
			setparam(mut->owner, curenv->priority);
		}
		spin_unlock(mut->prior_lock);
		if (time > 0) {
			add_to_waiting(mut, time, ENV_WAITING_WITH_TIME);
		} else {
			add_to_waiting(mut, 0, ENV_WAITING);
		}
	}
}

int s_check_after(int mut_id, int stat) {
	int id = decode(mut_id);
	if (id < 0) {
		return -1;
	}
	struct mutex *mut = &mut_array[id];
	if (!mut->allocated) {
		return -1;
	}
	if (stat && curenv->wait_time <= 0) {
		spin_lock(mut->prior_lock);
		if (mut->locked && mut->owner->priority == curenv->priority) {
			struct Env *tmp = mut->waiting_envs;
			unsigned max_prior_env = mut->owner_start_priority;
			while (tmp) {
				if (tmp->priority > max_prior_env) {
					max_prior_env = tmp->priority;
				}
				tmp = tmp->wait_next;
			}
			setparam(mut->owner,max_prior_env);
		}
		spin_unlock(mut->prior_lock);
		return -1;
	} else {
		return 0;
	}

}
void s_mutex_unlock(int mut_id) {
	int id = decode(mut_id);
	if (id < 0) {
		return;
	}
	struct mutex *mut = &mut_array[id];
	if (mut->owner != curenv || !mut->allocated) {
		return;
	}
	spin_lock(mut->prior_lock);
	setparam(curenv, mut->owner_start_priority);
	mut->owner_start_priority = 0;
	mut->owner = NULL;
	curenv->holding = -1;
	if (wake_one(mut) == -1) {
		xchg(&mut->locked, 0);
	}
	spin_unlock(mut->prior_lock);
	return;
}

int s_mutex_create() {
	if (!curenv->limit) {
		return -1;
	}
	int i = 0;
	for (; i < NENV; ++i) {
		if (mut_array[i].allocated == 0) {
			mut_array[i].locked = 0;
			int j = 0;
			for (; j < NENV; ++j) {
				if (spin_array[j].allocated == 0) {
					spin_array[j].allocated = 1;
					mut_array[i].prior_lock = &spin_array[j];
					break;
				}
			}
			if (j == NENV)	{
				return -1;
			}
			curenv->limit--;
			mut_array[i].allocated = 1;
			mut_array[i].creator_id = curenv->env_id;
			return code(i);
		}
	}
	return -1;
}

void s_mutex_delete(int mut_id) {
	int id = decode(mut_id);
	if (id < 0) {
		return;
	}
	struct mutex *mut = &mut_array[id];
	if (curenv->env_id != mut->creator_id || !mut->allocated) {
		return;
	}
	mut->owner = NULL;
	mut->allocated = 0;
	mut->prior_lock->allocated = 0;
	mut->prior_lock->locked = 0;
	mut->prior_lock = NULL;
	mut->creator_id = 0;
	curenv->limit++;
	wake_all(mut);
}
	
