// struct Pid_and_wasted_cycles{
// 	pid_t pid;
// 	long wasted_cycles;
// };

#include "../kernel/sched/sched.h"

asmlinkage long sys_balance1651(struct Pid_and_wasted_cycles worst_procs_all_cpus[][10]);
//asmlinkage long sys_balance1651(void);


void getWastedCycleInfo(struct Pid_and_wasted_cycles worst_procs_all_cpus[][10]){
	printk(KERN_INFO "1651 syscall in kernel");
	int history_size = 10;
	int i;
	int num_cpus = 4;
	for(i = 0; i < num_cpus; i++){
		printk(KERN_INFO "1651 grabbing info from rq %d", i);
		struct rq *the_rq = cpu_rq(i);
		int j;
		for(j = 0; j < history_size; j++){
			worst_procs_all_cpus[i][j].pid = the_rq->worst_procs[j].pid;
			worst_procs_all_cpus[i][j].wasted_cycles = the_rq->worst_procs[j].wasted_cycles;
		}
	}
}