#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/string.h>

#include "header_1.h"

#define NUM_OF_CORES 4
#define MIN_LOOP_ITERATIONS_COUNT 1000
#define INCREMENTOR 16384
#define CORE_0 0
#define CORE_1 1
#define CORE_2 2
#define CORE_3 3

static char* mode = "calibrate";
static char* run_mode = "run\n";
struct sched_param param;
module_param(mode, charp, 0644);

struct task* task_set[TASK_COUNT];
struct core* core_list[CPU_COUNT];

static struct task_struct *calibrate_task0;
static struct task_struct *calibrate_task1;
static struct task_struct *calibrate_task2;
static struct task_struct *calibrate_task3;
static int calibrate_thread_param;

// -1 means left goes first
// 1 means right goes first
// 0 means equivalent
static int utilization_comparator(const void* lhs, const void* rhs) {
	struct subtask* left = *(struct subtask**)(lhs);
	struct subtask* right = *(struct subtask**)(rhs);
	if (left->utilization > right->utilization) {
		return -1;
	}
	else if (left->utilization < right->utilization) {
		return 1;
	}
	else {
		return 0;
	}
}

// sort in decreasing relative deadline
// a.k.a. increasing priority
static int relative_deadline_comparator(const void* lhs, const void* rhs) {
	struct subtask left = *(struct subtask*)(lhs);
	struct subtask right = *(struct subtask*)(rhs);
	if (left.relative_deadline > right.relative_deadline) {
		return -1;
	}
	else if (left.relative_deadline < right.relative_deadline) {
		return 1;
	}
	else {
		return 0;
	}
}

void initialize(void) {
	printk(KERN_DEBUG "Assign tasks to task_set\n");
	task_set[TASK1_INDEX] = &first_task;
	task_set[TASK2_INDEX] = &second_task;
	task_set[TASK3_INDEX] = &third_task;
	int i, j;

	int count;
	count = 0;
	int task_execution_time;

	struct task* cur_task;
	struct subtask* cur_subtask;
	struct subtask* subtask_list[SUBTASK_COUNT];
	// build subtask_list and assign relative deadline
	printk(KERN_DEBUG "Start building subtask list\n");
	for (i = 0; i < TASK_COUNT; i++) {
			task_execution_time = 0;
			cur_task = task_set[i];
			printk(KERN_DEBUG "Working on task %d\n", i);
			for (j = 0; j < cur_task->subtask_count; j++) {
				cur_subtask = &cur_task->subtasks[j];
				// add to total
				task_execution_time += cur_subtask->execution_time;
				cur_subtask->cumulative_execution_time = task_execution_time;
				subtask_list[count] = cur_subtask;
				count++;
			}
			cur_task->execution_time = task_execution_time;
			// assign relative deadline
			for (j = 0; j < cur_task->subtask_count; j++) {
				cur_subtask = &cur_task->subtasks[j];
				cur_subtask->relative_deadline =
					cur_subtask->cumulative_execution_time * cur_task->period / cur_task->execution_time;
			}
	}
	printk(KERN_DEBUG "subtask list built\n");
	sort in decreasing order
	printk(KERN_DEBUG "Sorting based on utilization started\n");
	for (i = 0; i < SUBTASK_COUNT; i++) {
		printk(KERN_DEBUG "Subtask %d: name: %s, utilization %d\n", i, subtask_list[i]->name, subtask_list[i]->utilization);
	}

	// sort((void*)subtask_list, SUBTASK_COUNT, sizeof(struct subtask*), &utilization_comparator, NULL);
	// for (i = 0; i < SUBTASK_COUNT; i++) {
	// 	printk(KERN_DEBUG "Subtask %d: name: %s, utilization %d\n", i, subtask_list[i]->name, subtask_list[i]->utilization);
	// }
	// printk(KERN_DEBUG "Sorting based on utilization finished\n");
	// int cpu_load[CPU_COUNT] = {0, 0, 0, 0};
	// // assign cpu cores
	// int cpu_count[CPU_COUNT] = {0, 0, 0, 0};
	// printk(KERN_DEBUG "Start assigning CPU to subtask\n");
	// for (i = 0; i < SUBTASK_COUNT; i++) {
	// 	cur_subtask = subtask_list[i];
	// 	for (j = 0; j < CPU_COUNT; j++) {
	// 		// assign to the first available one
	// 		if (cpu_load[j] + cur_subtask->utilization < 100) {
	// 			cur_subtask->core = j;
	// 			cpu_count[j]++;
	// 			cpu_load[j] += cur_subtask->utilization;
	// 		}
	// 	}
	// 	// TODO: set default to -1
	// 	// not schedulable
	// 	// assign to core 0 for now, which is the same as start with 0
	// 	if (cur_subtask->core == -1) {
	// 		cur_subtask->core = 0;
	// 		cpu_count[0]++;
	// 	}
	// }
	// // build a struct core for each core
	// // with a list of subtasks
	// printk(KERN_DEBUG "Start building core lists\n");
	// for (i = 0; i < CPU_COUNT; i++) {
	// 	printk(KERN_DEBUG "Start building %d core list\n", i);
	// 	struct core* cur_core = kmalloc(sizeof(struct core) + sizeof(struct subtask) * cpu_count[i], GFP_KERNEL);
	// 	cur_core->subtask_count = cpu_count[i];
	// 	count = 0;
	// 	for (j = 0; j < SUBTASK_COUNT; j++) {
	// 		if (subtask_list[j]->core == i) {
	// 			cur_core->subtasks[count] = *subtask_list[j];
	// 			count++;
	// 		}
	// 	}
	// 	core_list[i] = cur_core;
	// }
	// // assign priority
	// printk(KERN_DEBUG "Start assigning priority to subtask\n");
	// for (i = 0; i < CPU_COUNT; i++) {
	// 	printk(KERN_DEBUG "Start building %d core list\n", i);
	// 	struct core* cur_core = core_list[i];
	// 	// sort
	// 	sort((void *)cur_core->subtasks, cur_core->subtask_count, sizeof(struct subtask), &relative_deadline_comparator, NULL);
	// 	for (j = 0; j < cur_core->subtask_count; j++) {
	// 		// increasing priority
	// 		cur_core->subtasks[j].priority = 3 * j;
	// 	}
	// }
}

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

// Step 6
// thread function for calibrate mode
// parameter: core number which can be used to find
// 		all subtasks assigned to this core
static int calibrate_thread(void * data) {
	int core_number =*((int*) data);
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
	struct subtask * subtasks = core_list[core_number]->subtasks;
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
			actual_execution_time=(int)(end_time.tv64-start_time.tv64);
			while(actual_execution_time>subtasks[i].execution_time){
				incrementor/=2;
				if(incrementor==0)break;
				next=current_count+incrementor;
				start_time=ktime_get();
				subtasks[i].loop_iterations_count=next;
				subtask_work(&subtasks[i]);
				end_time=ktime_get();
				actual_execution_time=(int)(end_time.tv64-start_time.tv64);
			}
			current_count+=incrementor;
		}
		printk(KERN_DEBUG "Core number is %d, subtask number is %d \n", core_number, i);
		printk(KERN_DEBUG "Task number is %d \n", subtasks[i].task_index);
		printk(KERN_DEBUG "subtask execution time is %d \n", subtasks[i].execution_time);
		printk(KERN_DEBUG "subtask utilization is %d \n", subtasks[i].utilization);
		printk(KERN_DEBUG "Loop iterations count is %d\n", subtasks[i].loop_iterations_count);
		printk(KERN_DEBUG "\n");
	}
	return 0;
}

struct subtask * subtask_lookup(struct hrtimer* hr_timer) {
	return container_of(&hr_timer, struct subtask, timer);
}

struct task * get_parent_task(struct subtask * task) {
	return container_of(
			(struct subtask *)(task - sizeof(struct subtask) * task->pos_in_task),
			struct task,
			subtasks[0]
	);
}

enum hrtimer_restart timer_expire(struct hrtimer* timer) {
	struct subtask * task = subtask_lookup(timer);
	if (task->task_struct_pointer != NULL) {
		wake_up_process(task->task_struct_pointer);
	}
	return HRTIMER_RESTART;
}

static int run_thread(void * data) {
	struct subtask * task = (struct subtask *)data;
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
	return 0;
}

int run_init(void) {
	int i, j;
	struct task* cur_mother_task;
	struct subtask* cur_subtask;
	printk(KERN_DEBUG "run inits.\n");
	for (i = 0; i < TASK_COUNT; i++) {
		cur_mother_task = task_set[i];
		for (j = 0; j < cur_mother_task->subtask_count; j++) {
				cur_subtask = &cur_mother_task->subtasks[j];
				cur_subtask->task_struct_pointer = kthread_create(run_thread, (void *)&cur_subtask, cur_subtask->name);
				kthread_bind(cur_subtask->task_struct_pointer, cur_subtask->core);
				param.sched_priority = cur_subtask->priority;
				sched_setscheduler(cur_subtask->task_struct_pointer, SCHED_FIFO, &param);
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
	struct subtask* cur_subtask;
	for (i = 0; i < TASK_COUNT; i++) {
		cur_mother_task = task_set[i];
		for (j = 0; j < cur_mother_task->subtask_count; j++) {
				cur_subtask = &cur_mother_task->subtasks[j];
				hrtimer_cancel(cur_subtask->timer);
				ret = kthread_stop(cur_subtask->task_struct_pointer);
				if (ret == 0) {
					printk(KERN_INFO "Subtask %s stopped", cur_subtask->name);
				}
		}
	}
}

int calibrate_init(void){
	printk(KERN_DEBUG "Calibrate inits.\n");
	calibrate_thread_param=CORE_0;
	calibrate_task0=kthread_create(calibrate_thread,(void *)&calibrate_thread_param,"core0");
	calibrate_thread_param=CORE_1;
	calibrate_task1=kthread_create(calibrate_thread,(void *)&calibrate_thread_param,"core1");
	calibrate_thread_param=CORE_2;
	calibrate_task2=kthread_create(calibrate_thread,(void *)&calibrate_thread_param,"core2");
	calibrate_thread_param=CORE_3;
	calibrate_task3=kthread_create(calibrate_thread,(void *)&calibrate_thread_param,"core3");

	kthread_bind(calibrate_task0,0);
	kthread_bind(calibrate_task1,0);
	kthread_bind(calibrate_task2,0);
	kthread_bind(calibrate_task3,0);

	param.sched_priority=0;
	sched_setscheduler(calibrate_task0,SCHED_FIFO,&param);
	sched_setscheduler(calibrate_task1,SCHED_FIFO,&param);
	sched_setscheduler(calibrate_task2,SCHED_FIFO,&param);
	sched_setscheduler(calibrate_task3,SCHED_FIFO,&param);

	wake_up_process(calibrate_task0);
	wake_up_process(calibrate_task1);
	wake_up_process(calibrate_task2);
	wake_up_process(calibrate_task3);

	return 0;
}

void calibrate_exit(void){
	printk(KERN_DEBUG "Calibrate exits.\n");
}

static int general_init(void) {
	int ret;
	printk(KERN_DEBUG "Mode is %s\n", mode);
	initialize();
	if (strncmp(mode, "run", 3) == 0) {
		ret = run_init();
	}
	else {
		ret = calibrate_init();
	}
	return ret;
}

static void general_exit(void) {
	if (strncmp(mode, "run", 3) == 0) {
		run_exit();
	}
	else {
		calibrate_exit();
	}
}

module_init(general_init);
module_exit(general_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Diqiu Zhou, Zimu Wang, Wei Weng");
MODULE_DESCRIPTION("Calibrate and run subtasks.");
MODULE_VERSION("1.0");
