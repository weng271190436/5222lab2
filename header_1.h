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

#define TASK_COUNT 3
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
	int schedule;
};

struct task {
	int period;
	int subtask_count;
	int execution_time;
	struct subtask subtasks[];
};

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
			0
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
			0
		}
	}
};

int getMin(int param[4]){
	int res;
        if(param[0] <= param[1]){
                if(param[0] <= param[2]){
                        if(param[0] <= param[3]){               	
				res = 0;
                        }else{
                                res = 3;
                        }
                }else if (param[2] <= param[3]){
                        res = 2;
                }else{
                        res = 3;
                }
        }else{
                 if(param[1] <= param[2]){
                        if(param[1] <= param[3]){
                                res = 1;
                        }else{
                                res = 3;
                        }
                }else if (param[2] <= param[3]){
                        res = 2;
                }else{
                        res = 3;
                }
        }
	if(param[res] >= 100){
		return 4;
	}else{
		return res;
	}
}

struct temp{
	int index;
	int ddl;
};


static int comparator(const void *p, const void *q) 
{
    int l = ((struct temp *)p)->ddl;
    int r = ((struct temp *)q)->ddl; 
    return (l - r);
}


struct temp* initialize(void) {
	task_set[0] = &first_task;
	//calculate core
	int i = 0;
	int util[4] = {0,0,0,0};
	while(i < first_task.subtask_count){
		int core = getMin(util);
		if (core != 4){
			first_task.subtasks[i].core = core;
                	util[core] += first_task.subtasks[i].utilization;
		}else{
			first_task.subtasks[i].core = 4;
			first_task.subtasks[i].schedule = 1;
		}
		//calculate deadline
		first_task.subtasks[i].relative_deadline = first_task.subtasks[i].cumulative_execution_time * TASK1_PERIOD / first_task.subtasks[i].execution_time;
		i = i + 1;
	}
	int max = 0;
	int j = 0;
	int k = 0;
	int count[4] = {0,0,0,0};
	//calculate priority
	for (j = 0; j < first_task.subtask_count; j = j + 1){
		if(first_task.subtasks[j].core != 4){
			count[first_task.subtasks[j].core]+=1;
		}
		for(k = 0; k < 4; k++){
			if(max < count[k]){
				max = count[k];
			}
		}
	}
	count[0] = 0;
	count[1] = 0;
	count[2] = 0;
	count[3] = 0;
	struct temp temp_list[4][max];
	for (j = 0; j < first_task.subtask_count; j = j + 1){
                if(first_task.subtasks[j].core != 4){
			struct temp result = {j, first_task.subtasks[i].relative_deadline};
                     	temp_list[first_task.subtasks[j].core][count[first_task.subtasks[j].core]] = result;
			count[first_task.subtasks[j].core] += 1;
                }
        }
	for (j = 0; j < 4; j++){
		int size = sizeof(temp_list[j]) / sizeof(temp_list[j][0]);
        	sort((void*)temp_list[j], size, sizeof(temp_list[j][0]), &comparator, NULL);
	}
	for (j = 0; j < 4; j++){
		for (k = 0; k < count[j]; k++){
			first_task.subtasks[temp_list[j][k].index].priority = max-k+1;
		}
	}
	return *temp_list;
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
