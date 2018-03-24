#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/kthread.h>

#include "header_1.h"

static char* mode = "calibrate";

static char* run_mode = "calibrate";

struct sched_param param;

module_param(mode, charp, 0644);

struct task* task_set[TASK_COUNT];

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

int run_init(void) {
	printk(KERN_DEBUG "run inits.\n");
	int i, j;
	struct task* cur_mother_task;
	struct subtask cur_subtask;
	for (i = 0; i < TASK_COUNT; i++) {
		cur_mother_task = task_set[i];
		for (j = 0; j < cur_mother_task->subtask_count; j++) {
				cur_subtask = cur_mother_task->subtasks[j];
				cur_subtask.task_struct_pointer = kthread_create(run_thread, &cur_subtask, cur_subtask.name);
				kthread_bind(cur_subtask.task_struct_pointer, cur_subtask.core);
				param.sched_priority = cur_subtask.priority;
				sched_setscheduler(cur_subtask.task_struct_pointer, SCHED_FIFO, &param);
		}
	}
	// waking up the first one;
	for (i = 0; i < TASK_COUNT; i++) {
		wake_up_process(task_set[i]->subtasks[0].task_struct_pointer);
	}
	return 0;
}

void run_exit(void) {
	printk(KERN_DEBUG "run exits.\n");
	int i, j, ret;
	struct task* cur_mother_task;
	struct subtask cur_subtask;
	for (i = 0; i < TASK_COUNT; i++) {
		cur_mother_task = task_set[i];
		for (j = 0; j < cur_mother_task->subtask_count; j++) {
				cur_subtask = cur_mother_task->subtasks[j];
				hrtimer_cancel(cur_subtask.timer);
				ret = kthread_stop(cur_subtask.task_struct_pointer);
				if (ret == 0) {
					printk(KERN_INFO "Subtask %d stopped", cur_subtask.name);
				}
		}
	}
}

int calibrate_init(void){
	printk(KERN_DEBUG "Calibrate inits.\n");
	return 0;
}

void calibrate_exit(void){
	printk(KERN_DEBUG "Calibrate exits.\n");
}

static int general_init(void) {
	printk(KERN_DEBUG "Mode is %s\n", mode);
	// initialize();
	int ret;
	if (strcmp(run_mode, "run")) {
		ret = run_init();
	}
	else {
		ret = calibrate_init();
	}
	return ret;
}

static void general_exit(void) {
	if (strcmp(run_mode, "run")) {
		run_exit();
	}
	else {
		calibrate_exit();
	}
	// TODO: clean up the remaining structures

}

module_init(general_init);
module_exit(general_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Diqiu Zhou, Zimu Wang, Wei Weng");
MODULE_DESCRIPTION("Calibrate and run subtasks.");
MODULE_VERSION("1.0");
