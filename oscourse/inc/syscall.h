#ifndef JOS_INC_SYSCALL_H
#define JOS_INC_SYSCALL_H

/* system call numbers */
enum {
	SYS_cputs = 0,
	SYS_cgetc,
	SYS_getenvid,
	SYS_env_destroy,
	SYS_page_alloc,
	SYS_page_map,
	SYS_page_unmap,
	SYS_exofork,
	SYS_env_set_status,
	SYS_env_set_trapframe,
	SYS_env_set_pgfault_upcall,
	SYS_yield,
	SYS_ipc_try_send,
	SYS_ipc_recv,
	SYS_gettime,
	SYS_sched_setparam,
	SYS_mutex_lock,
	SYS_mutex_unlock,
	SYS_mutex_create,
	SYS_mutex_delete,
	SYS_check_after,
	SYS_try_deny,
	NSYSCALLS
};

#endif /* !JOS_INC_SYSCALL_H */
