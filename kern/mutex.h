

struct mutex {
	unsigned locked;
	struct Env *owner;
	struct spinlock *prior_lock;
	struct Env *waiting_envs;
	int allocated;
	unsigned owner_start_priority;
	int32_t creator_id;
};
extern struct mutex mut_array[NENV];
void s_mutex_lock(int mut_id, int try, int time);
int s_check_after(int mut_id, int stat);
void s_mutex_unlock(int mut_id);
int s_mutex_create();
void s_mutex_delete(int mut_id);
size_t code(int i);
int decode(size_t i);
int extract_from_waiting(struct mutex *mut, struct Env *env);

