#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/timer.h>

#include "balance1651.h"


int g_time_interval = 250;//make the balancer run every 250ms (1/4 second)
struct timer_list g_timer;

/*restart the timer*/
void _TimerHandler(unsigned long data)
{
	printk(KERN_INFO "1651 Balance");
	ipc_balance();
	mod_timer(&g_timer, jiffies + msecs_to_jiffies(g_time_interval));
}


// asmlinkage long sys_balance1651(void){
// 	printk(KERN_INFO "1651 Balancer Starting");
// 	setup_timer(&g_timer, _TimerHandler, 0);
// 	mod_timer(&g_timer, jiffies + msecs_to_jiffies(g_time_interval));
// 	return 0;
// }

asmlinkage long sys_balance1651(struct Pid_and_wasted_cycles worst_procs_all_cpus[][10]){
	printk(KERN_INFO "1651 syscall starting");
	// int history_size = 10;
	// int i;
	// for(i = 0; i < 4; i++){

	// 	int j;
	// 	for(j = 0; j < history_size; j++){
	// 		worst_procs_all_cpus[i][j].pid = (i*10)+j;
	// 	}
	// }
	getWastedCycleInfo(worst_procs_all_cpus);
	return 0;
}

