/*
 *	Team members: Zimu Wang, Diqiu Zhou, Wei Weng
 *	
 *	The first set of tasks is manually calculated by Wei Weng
 *	It is used to test for easily schedulable tasks
 */

#include <stdlib.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/sched.h>

#define TASK_COUNT 3
#define SUBTASK_1_COUNT 2
#define SUBTASK_2_COUNT 3
#define SUBTASK_3_COUNT 4

#define NUM_ITERS_PER_MS 9804
#define 1_1_EXECUTION_TIME 10
#define 1_2_EXECUTION_TIME 10
#define 2_1_EXECUTION_TIME 10
#define 2_2_EXECUTION_TIME 10
#define 2_3_EXECUTION_TIME 10
#define 3_1_EXECUTION_TIME 10
#define 3_2_EXECUTION_TIME 10
#define 3_3_EXECUTION_TIME 10
#define 3_4_EXECUTION_TIME 10

#define TASK1_PERIOD 1000
#define TASK2_PERIOD 1000
#define TASK3_PERIOD 1000

#define TASK1_INDEX 0
#define TASK2_INDEX 1
#define TASK3_INDEX 2

#define SUBTASK1 0
#define SUBTASK2 1
#define SUBTASK3 2
#define SUBTASK4 3

struct subtask {
	struct hrtimer timer;
	struct task_struct* task_struct_pointer;
	ktime_t last_release_time;
	int loop_iterations_count;
	int cumulative_execution_time;
	int execution_time;
	float utilization;
	int core;
	int relative_deadline;
	int priority;
	int task_index;
};

struct task {
	struct subtask* subtasks;
	int period;
	int subtask_count;
	int execution_time;
};

// Declare and initialize hrtimers
// First task
struct hrtimer hr_timer_1_1;
hrtimer_init(&hr_timer_1_1,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
struct hrtimer hr_timer_1_2;
hrtimer_init(&hr_timer_1_2,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
// Second task
struct hrtimer hr_timer_2_1;
hrtimer_init(&hr_timer_2_1,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
struct hrtimer hr_timer_2_2;
hrtimer_init(&hr_timer_2_2,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
struct hrtimer hr_timer_2_3;
hrtimer_init(&hr_timer_2_3,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
// Third task
struct hrtimer hr_timer_3_1;
hrtimer_init(&hr_timer_3_1,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
struct hrtimer hr_timer_3_2;
hrtimer_init(&hr_timer_3_2,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
struct hrtimer hr_timer_3_3; 
hrtimer_init(&hr_timer_3_3,CLOCK_MONOTONIC,HRTIMER_MODE_REL);
struct hrtimer hr_timer_3_4; 
hrtimer_init(&hr_timer_3_5,CLOCK_MONOTONIC,HRTIMER_MODE_REL);

// Task set
struct task* task_set=
	(struct task*)malloc(sizeof(struct task)*TASK_COUNT);

// 1st task
task_set[TASK1_INDEX].subtasks=
	(struct subtask*)malloc(sizeof(struct subtask)*SUBTASK_1_COUNT);
// 1st task 1st subtask
task_set[TASK1_INDEX].subtasks[SUBTASK1]=
{
	hr_timer_1_1,//hrtimer
	NULL,//task_struct pointer
	0,//last release time,assign in runtime
	NUM_ITERS_PER_MS*1_1_EXECUTION_TIME,//loop iterations count
	0,//cumulative execution time,calculate later
	1_1_EXECUTION_TIME,//execution time
	1_1_EXECUTION_TIME/(float)TASK1_PERIOD,//utilization
	0,//core, calculate later
	0,//relative deadline, calculate later
	0,//priority, calculate later
	TASK1_INDEX//task index
};
// 1st task 2nd subtask
task_set[TASK1_INDEX].subtasks[SUBTASK2]=
{
	hr_timer_1_2,//hrtimer
	NULL,//task_struct pointer
	0,//last release time,assign in runtime
	NUM_ITERS_PER_MS*1_2_EXECUTION_TIME,//loop iterations count
	0,//cumulative execution time,calculate later
	1_2_EXECUTION_TIME,//execution time
	1_2_EXECUTION_TIME/(float)TASK1_PERIOD,//utilization
	0,//core, calculate later
	0,//relative deadline, calculate later
	0,//priority, calculate later
	TASK1_INDEX//task index
};
// 1st task period
task_set[TASK1_INDEX].period=TASK1_PERIOD;
// 1st task number of subtasks
task_set[TASK1_INDEX].subtask_count=SUBTASK_1_COUNT;
// 1st task execution time, calculate later
task_set[TASK1_INDEX].execution_time=0;

// 2nd task
task_set[TASK2_INDEX].subtasks=
	(struct subtask*)malloc(sizeof(struct subtask)*SUBTASK_2_COUNT);
// 2nd task 1st subtask
task_set[TASK2_INDEX].subtasks[SUBTASK1]=
{
	hr_timer_2_1,//hrtimer
	NULL,//task_struct pointer
	0,//last release time,assign in runtime
	NUM_ITERS_PER_MS*2_1_EXECUTION_TIME,//loop iterations count
	0,//cumulative execution time,calculate later
	2_1_EXECUTION_TIME,//execution time
	2_1_EXECUTION_TIME/(float)TASK2_PERIOD,//utilization
	0,//core, calculate later
	0,//relative deadline, calculate later
	0,//priority, calculate later
	0//task index
};
// 2nd task 2nd subtask
task_set[TASK2_INDEX].subtasks[SUBTASK2]=
{
	hr_timer_2_2,//hrtimer
	NULL,//task_struct pointer
	0,//last release time,assign in runtime
	NUM_ITERS_PER_MS*2_2_EXECUTION_TIME,//loop iterations count
	0,//cumulative execution time,calculate later
	2_2_EXECUTION_TIME,//execution time
	2_2_EXECUTION_TIME/(float)TASK2_PERIOD,//utilization
	0,//core, calculate later
	0,//relative deadline, calculate later
	0,//priority, calculate later
	TASK2_INDEX//task index
};
// 2nd task 3rd subtask
task_set[TASK2_INDEX].subtasks[SUBTASK3]=
{
	hr_timer_2_3,//hrtimer
	NULL,//task_struct pointer
	0,//last release time,assign in runtime
	NUM_ITERS_PER_MS*2_3_EXECUTION_TIME,//loop iterations count
	0,//cumulative execution time,calculate later
	2_3_EXECUTION_TIME,//execution time
	2_3_EXECUTION_TIME/(float)TASK3_PERIOD,//utilization
	0,//core, calculate later
	0,//relative deadline, calculate later
	0,//priority, calculate later
	TASK2_INDEX//task index
};
