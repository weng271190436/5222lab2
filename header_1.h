#pragma once
#ifndef HEADER_1_H
#define HEADER_1_H
/*
 *	Team members: Zimu Wang, Diqiu Zhou, Wei Weng
 *
 *	The first set of tasks is manually calculated by Wei Weng
 *	It is used to test for easily schedulable tasks
 */

#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/sort.h>
#include <linux/slab.h>

#define TASK_COUNT 3
#define SUBTASK_COUNT 9
#define SUBTASK_1_COUNT 2
#define SUBTASK_2_COUNT 3
#define SUBTASK_3_COUNT 4

#define NUM_ITERS_PER_MS 9804
#define ONE_1_EXECUTION_TIME 10
#define ONE_2_EXECUTION_TIME 10
#define TWO_1_EXECUTION_TIME 10
#define TWO_2_EXECUTION_TIME 10
#define TWO_3_EXECUTION_TIME 10
#define THREE_1_EXECUTION_TIME 10
#define THREE_2_EXECUTION_TIME 10
#define THREE_3_EXECUTION_TIME 10
#define THREE_4_EXECUTION_TIME 10

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

// "task1_subtask1"
#define NAME_BUFF 15

#define CPU_COUNT 4

struct subtask {
	struct hrtimer* timer;
	struct task_struct* task_struct_pointer;
	char name[NAME_BUFF];
	ktime_t last_release_time;
	int loop_iterations_count;
	int cumulative_execution_time;
	int execution_time;
	float utilization;
	int core;
	int relative_deadline;
	int priority;
	int task_index;
	int pos_in_task; // position in the bigger task
};

struct task {
	int period;
	int subtask_count;
	int execution_time;
	struct subtask subtasks[];
};

struct core {
	int subtask_count;
	struct subtask subtasks[];
};

extern struct core* core_list[CPU_COUNT];

// Declare and initialize hrtimers
// First task
struct hrtimer hr_timer_1_1;
struct hrtimer hr_timer_1_2;
// Second task
struct hrtimer hr_timer_2_1;
struct hrtimer hr_timer_2_2;
struct hrtimer hr_timer_2_3;
// Third task
struct hrtimer hr_timer_3_1;
struct hrtimer hr_timer_3_2;
struct hrtimer hr_timer_3_3;
struct hrtimer hr_timer_3_4;

// Task set
extern struct task* task_set[TASK_COUNT];
struct task first_task=
{
	TASK1_PERIOD,
	SUBTASK_1_COUNT,
	0, // execution time, calculate later
	{
		// Subtask 1
		{
			NULL,//hrtimer
			NULL,//task_struct pointer
			"task1_subtask1", // name
			(int)0,//last release time,assign in runtime
			NUM_ITERS_PER_MS*ONE_1_EXECUTION_TIME,//loop iterations count
			0,//cumulative execution time,calculate later
			ONE_1_EXECUTION_TIME,//execution time
			ONE_1_EXECUTION_TIME/(float)TASK1_PERIOD,//utilization
			0,//core, calculate later
			0,//relative deadline, calculate later
			0,//priority, calculate later
			TASK1_INDEX,//task index
		},
		// Subtask 2
		{
			NULL,//hrtimer
			NULL,//task_struct pointer
			"task1_subtask2", // name
			(int)0,//last release time,assign in runtime
			NUM_ITERS_PER_MS*ONE_2_EXECUTION_TIME,//loop iterations count
			0,//cumulative execution time,calculate later
			ONE_2_EXECUTION_TIME,//execution time
			ONE_2_EXECUTION_TIME/(float)TASK1_PERIOD,//utilization
			0,//core, calculate later
			0,//relative deadline, calculate later
			0,//priority, calculate later
			TASK1_INDEX,//task index
		}
	}
};

struct task second_task=
{
	TASK2_PERIOD,
	SUBTASK_2_COUNT,
	0, // execution time, calculate later
	{
		// Subtask 1
		{
			NULL,//hrtimer
			NULL,//task_struct pointer
			"task2_subtask1", // name
			(int)0,//last release time,assign in runtime
			NUM_ITERS_PER_MS*TWO_1_EXECUTION_TIME,//loop iterations count
			0,//cumulative execution time,calculate later
			TWO_1_EXECUTION_TIME,//execution time
			TWO_1_EXECUTION_TIME/(float)TASK2_PERIOD,//utilization
			0,//core, calculate later
			0,//relative deadline, calculate later
			0,//priority, calculate later
			TASK2_INDEX,//task index
		},
		// Subtask 2
		{
			NULL,//hrtimer
			NULL,//task_struct pointer
			"task2_subtask2", // name
			(int)0,//last release time,assign in runtime
			NUM_ITERS_PER_MS*TWO_2_EXECUTION_TIME,//loop iterations count
			0,//cumulative execution time,calculate later
			TWO_2_EXECUTION_TIME,//execution time
			TWO_2_EXECUTION_TIME/(float)TASK2_PERIOD,//utilization
			0,//core, calculate later
			0,//relative deadline, calculate later
			0,//priority, calculate later
			TASK2_INDEX,//task index
		},
		// Subtask 3
		{
			NULL,//hrtimer
			NULL,//task_struct pointer
			"task2_subtask3", // name
			(int)0,//last release time,assign in runtime
			NUM_ITERS_PER_MS*TWO_3_EXECUTION_TIME,//loop iterations count
			0,//cumulative execution time,calculate later
			TWO_3_EXECUTION_TIME,//execution time
			TWO_3_EXECUTION_TIME/(float)TASK2_PERIOD,//utilization
			0,//core, calculate later
			0,//relative deadline, calculate later
			0,//priority, calculate later
			TASK2_INDEX,//task index
		}
	}
};

struct task third_task=
{
	TASK3_PERIOD,
	SUBTASK_3_COUNT,
	0, // execution time, calculate later
	{
		// Subtask 1
		{
			NULL,//hrtimer
			NULL,//task_struct pointer
			"task3_subtask1", // name
			(int)0,//last release time,assign in runtime
			NUM_ITERS_PER_MS*THREE_1_EXECUTION_TIME,//loop iterations count
			0,//cumulative execution time,calculate later
			THREE_1_EXECUTION_TIME,//execution time
			THREE_1_EXECUTION_TIME/(float)TASK3_PERIOD,//utilization
			0,//core, calculate later
			0,//relative deadline, calculate later
			0,//priority, calculate later
			TASK3_INDEX,//task index
		},
		// Subtask 2
		{
			NULL,//hrtimer
			NULL,//task_struct pointer
			"task3_subtask2", // name
			(int)0,//last release time,assign in runtime
			NUM_ITERS_PER_MS*THREE_2_EXECUTION_TIME,//loop iterations count
			0,//cumulative execution time,calculate later
			THREE_2_EXECUTION_TIME,//execution time
			THREE_2_EXECUTION_TIME/(float)TASK3_PERIOD,//utilization
			0,//core, calculate later
			0,//relative deadline, calculate later
			0,//priority, calculate later
			TASK3_INDEX,//task index
		},
		// Subtask 3
		{
			NULL,//hrtimer
			NULL,//task_struct pointer
			"task3_subtask3", // name
			(int)0,//last release time,assign in runtime
			NUM_ITERS_PER_MS*THREE_3_EXECUTION_TIME,//loop iterations count
			0,//cumulative execution time,calculate later
			THREE_3_EXECUTION_TIME,//execution time
			THREE_3_EXECUTION_TIME/(float)TASK3_PERIOD,//utilization
			0,//core, calculate later
			0,//relative deadline, calculate later
			0,//priority, calculate later
			TASK3_INDEX,//task index
		},
		// Subtask 4
		{
			NULL,//hrtimer
			NULL,//task_struct pointer
			"task3_subtask4", // name
			(int)0,//last release time,assign in runtime
			NUM_ITERS_PER_MS*THREE_4_EXECUTION_TIME,//loop iterations count
			0,//cumulative execution time,calculate later
			THREE_4_EXECUTION_TIME,//execution time
			THREE_4_EXECUTION_TIME/(float)TASK3_PERIOD,//utilization
			0,//core, calculate later
			0,//relative deadline, calculate later
			0,//priority, calculate later
			TASK3_INDEX,//task index
		}
	}
};

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
	task_set[TASK1_INDEX] = &first_task;
	task_set[TASK2_INDEX] = &second_task;
	task_set[TASK3_INDEX] = &third_task;
	int i, j;

	int count;
	count = 0;
	int task_execution_time;

	struct task* cur_task;
	struct subtask cur_subtask;
	struct subtask* subtask_list[SUBTASK_COUNT];
	
	// build subtask_list and assign relative deadline
	for (i = 0; i < TASK_COUNT; i++) {
			task_execution_time = 0;
			cur_task = task_set[i];
			for (j = 0; j < cur_task->subtask_count; j++) {
				cur_subtask = cur_task->subtasks[j];
				// add to total
				task_execution_time += cur_subtask.execution_time;
				cur_subtask.cumulative_execution_time = task_execution_time;
				subtask_list[count] = &cur_subtask;
			}
			cur_task->execution_time = task_execution_time;
			// assign relative deadline
			for (j = 0; j < cur_task->subtask_count; j++) {
				cur_subtask = cur_task->subtasks[j];
				cur_subtask.relative_deadline =
					cur_subtask.cumulative_execution_time * cur_task->period / cur_task->execution_time;
			}
	}
	// sort in decreasing order
	sort((void*)subtask_list, SUBTASK_COUNT, sizeof(struct subtask*), &utilization_comparator, NULL);
	int cpu_load[CPU_COUNT] = {0, 0, 0, 0};
	// assign cpu cores
	int cpu_count[CPU_COUNT] = {0, 0, 0, 0};
	for (i = 0; i < SUBTASK_COUNT; i++) {
		cur_subtask = *subtask_list[i];
		for (j = 0; j < CPU_COUNT; j++) {
			// assign to the first available one
			if (cpu_load[j] + cur_subtask.utilization < 100) {
				cur_subtask.core = j;
				cpu_count[j]++;
				cpu_load[j] += cur_subtask.utilization;
			}
		}
		// TODO: set default to -1
		// not schedulable
		// assign to core 0 for now, which is the same as start with 0
		if (cur_subtask.core == -1) {
			cur_subtask.core = 0;
			cpu_count[0]++;
		}
	}
	// build a struct core for each core
	// with a list of subtasks
	for (i = 0; i < CPU_COUNT; i++) {
		struct core* cur_core = kmalloc(sizeof(struct core) + sizeof(struct subtask) * cpu_count[i], GFP_KERNEL);
		cur_core->subtask_count = cpu_count[i];
		int count = 0;
		for (j = 0; j < SUBTASK_COUNT; j++) {
			if (subtask_list[j]->core == i) {
				cur_core->subtasks[count] = *subtask_list[j];
				count++;
			}
		}
		core_list[i] = cur_core;
	}
	// assign priority
	for (i = 0; i < CPU_COUNT; i++) {
		struct core* cur_core = core_list[i];
		// sort
		sort((void *)cur_core->subtasks, cur_core->subtask_count, sizeof(struct subtask), &relative_deadline_comparator, NULL);
		for (j = 0; j < cur_core->subtask_count; j++) {
			// increasing priority
			cur_core->subtasks[j].priority = 3 * j;
		}
	}
}

/*
{
	// Task 1
	{
		{
			// Subtask 1
			{
				hr_timer_1_1,//hrtimer
				NULL,//task_struct pointer
				0,//last release time,assign in runtime
				NUM_ITERS_PER_MS*ONE_1_EXECUTION_TIME,//loop iterations count
				0,//cumulative execution time,calculate later
				ONE_1_EXECUTION_TIME,//execution time
				ONE_1_EXECUTION_TIME/(float)TASK1_PERIOD,//utilization
				0,//core, calculate later
				0,//relative deadline, calculate later
				0,//priority, calculate later
				TASK1_INDEX//task index
			},
			// Subtask 2
			{
				hr_timer_1_2,//hrtimer
				NULL,//task_struct pointer
				0,//last release time,assign in runtime
				NUM_ITERS_PER_MS*ONE_2_EXECUTION_TIME,//loop iterations count
				0,//cumulative execution time,calculate later
				ONE_2_EXECUTION_TIME,//execution time
				ONE_2_EXECUTION_TIME/(float)TASK1_PERIOD,//utilization
				0,//core, calculate later
				0,//relative deadline, calculate later
				0,//priority, calculate later
				TASK1_INDEX//task index
			}
		},
		TASK1_PERIOD,
		SUBTASK_1_COUNT,
		0 // execution time, calculate later
	},

	// Task 2
	{

		{
			// Subtask 1
			{
				hr_timer_2_1,//hrtimer
				NULL,//task_struct pointer
				0,//last release time,assign in runtime
				NUM_ITERS_PER_MS*TWO_1_EXECUTION_TIME,//loop iterations count
				0,//cumulative execution time,calculate later
				TWO_1_EXECUTION_TIME,//execution time
				TWO_1_EXECUTION_TIME/(float)TASK2_PERIOD,//utilization
				0,//core, calculate later
				0,//relative deadline, calculate later
				0,//priority, calculate later
				TASK2_INDEX//task index
			},
			// Subtask 2
			{
				hr_timer_2_2,//hrtimer
				NULL,//task_struct pointer
				0,//last release time,assign in runtime
				NUM_ITERS_PER_MS*TWO_2_EXECUTION_TIME,//loop iterations count
				0,//cumulative execution time,calculate later
				TWO_2_EXECUTION_TIME,//execution time
				TWO_2_EXECUTION_TIME/(float)TASK2_PERIOD,//utilization
				0,//core, calculate later
				0,//relative deadline, calculate later
				0,//priority, calculate later
				TASK2_INDEX//task index
			},

			// Subtask 3
			{
				hr_timer_2_3,//hrtimer
				NULL,//task_struct pointer
				0,//last release time,assign in runtime
				NUM_ITERS_PER_MS*TWO_3_EXECUTION_TIME,//loop iterations count
				0,//cumulative execution time,calculate later
				TWO_3_EXECUTION_TIME,//execution time
				TWO_3_EXECUTION_TIME/(float)TASK2_PERIOD,//utilization
				0,//core, calculate later
				0,//relative deadline, calculate later
				0,//priority, calculate later
				TASK2_INDEX//task index
			}
		},
		TASK2_PERIOD,
		SUBTASK_2_COUNT,
		0 // execution time, calculate later
	},

	// Task 3
	{

		{
			// Subtask 1
			{
				hr_timer_3_1,//hrtimer
				NULL,//task_struct pointer
				0,//last release time,assign in runtime
				NUM_ITERS_PER_MS*THREE_1_EXECUTION_TIME,//loop iterations count
				0,//cumulative execution time,calculate later
				THREE_1_EXECUTION_TIME,//execution time
				THREE_1_EXECUTION_TIME/(float)TASK3_PERIOD,//utilization
				0,//core, calculate later
				0,//relative deadline, calculate later
				0,//priority, calculate later
				TASK3_INDEX//task index
			},
			// Subtask 2
			{
				hr_timer_3_2,//hrtimer
				NULL,//task_struct pointer
				0,//last release time,assign in runtime
				NUM_ITERS_PER_MS*THREE_2_EXECUTION_TIME,//loop iterations count
				0,//cumulative execution time,calculate later
				THREE_2_EXECUTION_TIME,//execution time
				THREE_2_EXECUTION_TIME/(float)TASK3_PERIOD,//utilization
				0,//core, calculate later
				0,//relative deadline, calculate later
				0,//priority, calculate later
				TASK3_INDEX//task index
			},

			// Subtask 3
			{
				hr_timer_3_3,//hrtimer
				NULL,//task_struct pointer
				0,//last release time,assign in runtime
				NUM_ITERS_PER_MS*THREE_3_EXECUTION_TIME,//loop iterations count
				0,//cumulative execution time,calculate later
				THREE_3_EXECUTION_TIME,//execution time
				THREE_3_EXECUTION_TIME/(float)TASK3_PERIOD,//utilization
				0,//core, calculate later
				0,//relative deadline, calculate later
				0,//priority, calculate later
				TASK3_INDEX//task index
			},

			// Subtask 4
			{
				hr_timer_3_4,//hrtimer
				NULL,//task_struct pointer
				0,//last release time,assign in runtime
				NUM_ITERS_PER_MS*THREE_4_EXECUTION_TIME,//loop iterations count
				0,//cumulative execution time,calculate later
				THREE_4_EXECUTION_TIME,//execution time
				THREE_4_EXECUTION_TIME/(float)TASK3_PERIOD,//utilization
				0,//core, calculate later
				0,//relative deadline, calculate later
				0,//priority, calculate later
				TASK3_INDEX//task index
			}
		},
		TASK3_PERIOD,
		SUBTASK_3_COUNT,
		0 // execution time, calculate later

	}
}


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
*/
#endif
