#include "../kernel/sched/sched.h"
#include "MyDef.h"

asmlinkage long sys_balance1651(struct Pid_and_wasted_cycles worst_procs_all_cpus[][HISTORY_SIZE_1651]);
//asmlinkage long sys_balance1651(void);


void getWastedCycleInfo(struct Pid_and_wasted_cycles worst_procs_all_cpus[][HISTORY_SIZE_1651]){
	printk(KERN_INFO "1651 syscall in kernel");
	int i;
	for(i = 0; i < NUM_CPUS_1651; i++){
		printk(KERN_INFO "1651 grabbing info from rq %d", i);
		struct rq *the_rq = cpu_rq(i);
		int j;
		for(j = 0; j < HISTORY_SIZE_1651; j++){
			worst_procs_all_cpus[i][j].pid = the_rq->worst_procs[j].pid;
			worst_procs_all_cpus[i][j].wasted_cycles = the_rq->worst_procs[j].wasted_cycles;
		}
	}
}