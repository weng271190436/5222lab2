#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/kthread.h>

#include "header_1.h"

#define NUM_OF_CORES 4
#define MIN_LOOP_ITERATIONS_COUNT 1000
#define INCREMENTOR 15000

static char* mode = "calibrate";
module_param(mode, charp, 0644);

void* task_set[TASK_COUNT];

struct sched_param param;

// busy looping in the subtask
void subtask_work(struct subtask* task) {
	int i;
	for (i = 0; i < task->loop_iterations_count; i++) {
		ktime_get();
	}
}

// Step 6
// thread function for calibrate mode
// parameter: core number which can be used to find
// 		all subtasks assigned to this core
static int calibrate_mode_threadfunc(int core_number) {
	if (core_number < 0 || core_number > NUM_OF_CORES) return -1;
	//set_current_state(TASK_INTERRUPTIBLE);
	//schedule();

	// After waking up
	//printk(KERN_DEBUG "Core %d has just woken up.\n", core_number);

	//TODO: sets its priority to the priority specified in its subtask
	//	struct

	// Binary search for the maximum number of loop iterations that can
	// be run nwithout excedding the subtask's specified execution
	// time
	struct subtask * subtasks = subtasks_by_core[core_number];
	int num_of_subtasks = (int) sizeof(subtasks)/sizeof(subtasks[0]);
	int i;
	for (i=0;i<num_of_subtasks;i++){
		param.sched_priority=subtasks[i].priority;
		sched_setscheduler(current,SCHED_FIFO,&param);
		//schedule();

		// Calibrate
		int incrementor=INCREMENTOR;
		int current_count=MIN_LOOP_ITERATIONS_COUNT;
		ktime_t start_time;
		ktime_t end_time;
		int actual_execution_time;
		int next;
		while(incrementor>0){
			next=current_count+incrementor;
			start_time=ktime_get();
			subtasks[i].loop_iterations_count=next;
			subtask_work(&subtasks[i]);
			end_time=ktime_get();
			actual_execution_time=end_time-start_time;
			while(actual_exection_time>subtasks[i].execution_time){
				incrementor/=2;
				if(incrementor==0)break;
				next=current_count+incrementor;
				start_time=ktime_get();
				subtasks[i].loop_iterations_count=next;
				subtask_work(&subtasks[i]);
				end_time=ktime_get();
				actual_execution_time=end_time-start_time;
			}
			current_count+=incrementor;
		}

	}
}

struct subtask * subtask_lookup(struct hrtimer* hr_timer) {
	return container_of(&hr_timer, struct subtask, timer);
}

struct task * get_parent_task(struct subtask * task) {
	return container_of(
			(struct subtask *)(task - sizeof(struct subtask) * task->pos_in_task),
			struct task,
			subtasks[0],
	);
}

enum hrtimer_restart timer_expire(struct hrtimer* timer) {
	struct subtask * task = subtask_lookup(timer);
	if (task->task_struct_pointer != NULL) {
		wake_up_process(task->task_struct_pointer);
	}
	return HRTIMER_RESTART;
}

/*
static run_thread(struct subtask* task) {
	hrtimer_init(&task->timer, CLOCK_MONOTONIC, HRTIMER_MODE_ABS);
	&task->timer.function = &timer_expire;
	while (!kthread_should_stop()) {
		set_current_state(TASK_INTERRUPTIBLE);
		schedule();

		if (kthread_should_stop()) {
			break;
		}

		task->last_release_time = ktime_get();

		subtask_work(task);

		if (task->pos_in_task == 0) {
			ktime_t period;
			struct task* parent_task = get_parent_task(task);
			period = ktime_set(0, parent_task->period);
		}
	}
}
*/

static int calibrate_init(void){
	printk(KERN_INFO "Mode is %s\n", mode);
	return 0;
}

static void calibrate_exit(void){
	printk(KERN_INFO "Calibrate exits.\n");
}

module_init(calibrate_init);
module_exit(calibrate_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Diqiu Zhou, Zimu Wang, Wei Weng");
MODULE_DESCRIPTION("Calibrate and run subtasks.");
MODULE_VERSION("1.0");
