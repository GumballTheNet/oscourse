//#include <kern/mutex.h>
struct mutex {
	unsigned locked;
	struct Env *owner;
	struct spinlock *prior_lock;
	struct Env *waiting_envs;
	int allocated;
};/*
int mutex_lock(struct mutex *mut, int try, int time);
void mutex_unlock(struct mutex *mut);
struct mutex* mutex_create();
void mutex_delete(struct mutex *mut);*/
