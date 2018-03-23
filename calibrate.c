#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/kthread.h>

#include <./calibrate_header.h>

static char* mode = "calibrate";
module_param(mode, charp, 0644);

/*
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

struct subtask * subtask_lookup(struct hrtimer* timer) {
	return container_of(timer, struct subtask, timer);
}

void*

enum hrtimer_restart timer_expire(struct hrtimer* timer) {
	struct subtask * task = subtask_lookup(timer);
	wake_up_process(task);
	return HRTIMER_RESTART;
}

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
