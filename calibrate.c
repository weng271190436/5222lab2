#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/kthread.h>

#include "header_1.h"

static char* mode = "calibrate";
module_param(mode, charp, 0644);

void* task_set[TASK_COUNT];

// busy looping in the subtask
void subtask_work(struct subtask* task) {
	int i;
	for (i = 0; i < task->loop_iterations_count; i++) {
		ktime_get();
	}
}

// TODO: parameter: list of subtasks on the same core
void calibrate_core(void) {
	// do nothing
}

struct subtask * subtask_lookup(struct hrtimer* hr_timer) {
	return container_of(&hr_timer, struct subtask, timer);
}

struct task * get_parent_task(struct subtask * task) {
	return container_of((struct subtask *)(task - sizeof(struct subtask) * task->pos_in_task), struct task, subtasks[0]);
}

enum hrtimer_restart timer_expire(struct hrtimer* timer) {
	struct subtask * task = subtask_lookup(timer);
	if (task->task_struct_pointer != NULL) {
		wake_up_process(task->task_struct_pointer);
	}
	return HRTIMER_RESTART;
}


static void run_thread(struct subtask* task) {
	hrtimer_init(task->timer, CLOCK_MONOTONIC, HRTIMER_MODE_ABS);
	task->timer->function = &timer_expire;
	while (!kthread_should_stop()) {
		set_current_state(TASK_INTERRUPTIBLE);
		schedule();

		if (kthread_should_stop()) {
			break;
		}

		task->last_release_time = ktime_get();

		subtask_work(task);

		struct task* parent_task = get_parent_task(task);
		ktime_t period;
		period = ktime_set(0, parent_task->period);
		// schedule next wakeup
		if (task->pos_in_task == 0) {
			hrtimer_forward(task->timer, task->last_release_time, period);
		}
		// schedule next subtask
		else if ((task->pos_in_task != parent_task->subtask_count - 1)) {
			struct subtask next_subtask = parent_task->subtasks[task->pos_in_task + 1];
			ktime_t cur_time = ktime_get();
			ktime_t next_wakeup = ktime_add(next_subtask.last_release_time, period);
			if (ktime_before(cur_time, next_wakeup)) {
					// schedule
					hrtimer_forward(next_subtask.timer, next_subtask.last_release_time, period);
			}
			else {
					// wake up
					wake_up_process(next_subtask.task_struct_pointer);
			}
		}
	}
}


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
