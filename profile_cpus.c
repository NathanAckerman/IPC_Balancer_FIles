#define _GNU_SOURCE
#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>


#include <stdlib.h>
#include <sched.h>
#include <limits.h>

#define HISTORY_SIZE 20
#define NUM_CPUS 4

struct Pid_and_wasted_cycles{
	pid_t pid;
	long wasted_cycles;
};


//function to total up the wasted cycles of all the worst procs of the cpu
long long get_cpu_total_wasted(int cpu_num, struct Pid_and_wasted_cycles worst_procs_all_cpus[][HISTORY_SIZE])
{
	long long total = 0;
	int i;
	for(i = 0; i < HISTORY_SIZE; i++){
		//printk(KERN_INFO "CS1651 Totalling cpu: %d pid: %d wasted: %ld", cpu_num, the_rq->worst_procs[i].pid, the_rq->worst_procs[i].wasted_cycles);
		if(worst_procs_all_cpus[cpu_num][i].pid != -1){
			total += worst_procs_all_cpus[cpu_num][i].wasted_cycles;
		}
	}
	//printk(KERN_INFO "CS1651 cpu: %d cycles_wasted: %lld", cpu_num, total);
	return total;
}


//set the specified tasks affinity to the cpu with the least wasted cycles
//the task came from the cpu with the most wasted cycles
long transfer_to_min(int min_cpu_num, pid_t task_to_move){
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(min_cpu_num, &mask);
	return sched_setaffinity(task_to_move, sizeof(mask), &mask);
}

//loop through the procs on the cpu with the most wasted cycles and set them to the min cpu
void balance_min_and_max(int cpu_min, int cpu_max, long long cpu_wasted_cycles[NUM_CPUS], struct Pid_and_wasted_cycles worst_procs_all_cpus[][HISTORY_SIZE]){
	// struct rq *rq_min = cpu_rq(cpu_min);
	// struct rq *rq_max = cpu_rq(cpu_max);
    long min_wasted_cycles = cpu_wasted_cycles[cpu_min];
    long max_wasted_cycles = cpu_wasted_cycles[cpu_max];
    int i = 0;//make sure we only transfer a max of the 10 worst procs
    //printf("max: %ld min: %ld\n", max_wasted_cycles, min_wasted_cycles);
    while(i < HISTORY_SIZE && max_wasted_cycles > min_wasted_cycles){
    	pid_t pid = worst_procs_all_cpus[cpu_max][i].pid;
		if(pid > 1){
			long cycles_of_task_to_transfer = worst_procs_all_cpus[cpu_max][i].wasted_cycles;
			if(transfer_to_min(cpu_min, pid) >= 0){
				//printf("CS1651 Transfer %d to %d\n", pid, cpu_min);
				max_wasted_cycles -=  cycles_of_task_to_transfer;
				min_wasted_cycles +=  cycles_of_task_to_transfer;
			}
		}	
		i++;
    }
}

//function to do the actual load balancing and move a process
void ipc_balance(struct Pid_and_wasted_cycles worst_procs_all_cpus[][HISTORY_SIZE])
{
	int min_cpu = 0;
	long long min_cpu_wasted_cycles = LLONG_MAX;
	int max_cpu = 0;
	long long max_cpu_wasted_cycles = -1;
	//array of 2 element arrays which hold a cpu's total wasted cycles and the worst proc's pid
	long long cpu_wasted_cycles[NUM_CPUS];
	int i;
	for(i = 0; i < NUM_CPUS; i++){
		long long wasted = get_cpu_total_wasted(i, worst_procs_all_cpus);
		cpu_wasted_cycles[i] = wasted;
		// if(wasted < min_cpu_wasted_cycles){
		// 	min_cpu = i;
		// 	min_cpu_wasted_cycles = wasted;
		// }
		// if(wasted > max_cpu_wasted_cycles){
		// 	max_cpu = i;
		// 	max_cpu_wasted_cycles = wasted;
		// }
	}
	FILE *f;
	f = fopen("cpu_data.txt", "a");
	for(i = 0; i < NUM_CPUS; i++){
		//char output[sizeof(long long)];
		fprintf(f, "%lld:", cpu_wasted_cycles[i]);
	}
	fprintf(f, "\n");
	fclose(f);

	//printf("CS1651 MIN CPU: %d Cycles: %lld\n", min_cpu, min_cpu_wasted_cycles);
	//printf("CS1651 MAX CPU: %d Cycles: %lld\n", max_cpu, max_cpu_wasted_cycles);
	//dont balance in the profiler
	//balance_min_and_max(min_cpu, max_cpu, cpu_wasted_cycles, worst_procs_all_cpus);
}

int main(){
	while(1){	
		struct Pid_and_wasted_cycles worst_procs_all_cpus[NUM_CPUS][HISTORY_SIZE];//first index is cpu num
		long ret = syscall(332, worst_procs_all_cpus);
		int i;
		// for(i = 0; i < NUM_CPUS; i++){
		// 	printf("cpu: %d\n", i);
		// 	int j;
		// 	for(j = 0; j < HISTORY_SIZE; j++){
		// 		printf("pid: %d cycles: %ld\n", worst_procs_all_cpus[i][j].pid, worst_procs_all_cpus[i][j].wasted_cycles);
		// 	}
		// 	printf("\n\n");
		// }
		ipc_balance(worst_procs_all_cpus);
		//printf("\n\n******************\n\n");


		


		sleep(1);
	}
}
