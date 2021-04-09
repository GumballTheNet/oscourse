#include <inc/assert.h>
#include <inc/x86.h>
#include <inc/env.h>
#include <kern/mutex.h>
#include <kern/env.h>
#include <kern/monitor.h>

struct Taskstate cpu_ts;
void sched_halt(void);

// Choose a user environment to run and run it.
void
sched_yield(void)
{
	if (curenv && (heads[curenv->priority] != lists[curenv->priority] && curenv != lists[curenv->priority])) {
		if (extract_env(curenv) != -1)
			add_env(curenv);
        }
	for (int i = PRIOR_COUNT - 1; i >= 0; --i) {
		if (heads[i]) {
			env_run(heads[i]);
		}
	}
	struct Env *to_wake = NULL;
	for (int i = 0; i < NENV; ++i) {
		if (envs[i].env_status == ENV_WAITING_WITH_TIME && (!to_wake || envs[i].priority > to_wake->priority)) {
			to_wake = &envs[i];
		}
	}
	if (to_wake && to_wake->holding != -1) {
		to_wake->wait_time = -1;
		extract_from_waiting(&mut_array[decode(to_wake->holding)], to_wake);
		env_run(to_wake);
	}
	sched_halt();
}

// Halt this CPU when there is nothing to do. Wait until the
// timer interrupt wakes it up. This function never returns.
//
void
sched_halt(void)
{
	int i;

	// For debugging and testing purposes, if there are no runnable
	// environments in the system, then drop into the kernel monitor.
	for (i = 0; i < NENV; i++) {
		if ((envs[i].env_status == ENV_RUNNABLE ||
		     envs[i].env_status == ENV_RUNNING ||
		     envs[i].env_status == ENV_DYING))
			break;
	}
	if (i == NENV) {
		cprintf("No runnable environments in the system!\n");
		while (1)
			monitor(NULL);
	}
	// Mark that no environment is running on CPU
	curenv = NULL;

	// Reset stack pointer, enable interrupts and then halt.
	asm volatile (
		"movl $0, %%ebp\n"
		"movl %0, %%esp\n"
		"pushl $0\n"
		"pushl $0\n"
		"sti\n"
		"hlt\n"
	: : "a" (cpu_ts.ts_esp0));
}

