#pragma once
#ifndef HEADER_2_H
#define HEADER_2_H
/*
 *    Team members: Zimu Wang, Diqiu Zhou, Wei Weng
 *
 *    The second set of tasks is manually calculated by Diqiu Zhou
 *    It contains unscheduable tasks
 */

#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/sort.h>
#include <linux/slab.h>


#define TASK_COUNT 3
#define SUBTASK_COUNT 6
#define SUBTASK_1_COUNT 2
#define SUBTASK_2_COUNT 2
#define SUBTASK_3_COUNT 2

#define NUM_ITERS_PER_MS 9804
#define ONE_1_EXECUTION_TIME 78
#define ONE_2_EXECUTION_TIME 79
#define TWO_1_EXECUTION_TIME 80
#define TWO_2_EXECUTION_TIME 81
#define THREE_1_EXECUTION_TIME 82
#define THREE_2_EXECUTION_TIME 83

#define TASK1_PERIOD 100
#define TASK2_PERIOD 100
#define TASK3_PERIOD 100

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
    void* parent_task;
    char name[NAME_BUFF];
    ktime_t last_release_time;
    int loop_iterations_count;
    int cumulative_execution_time;
    int execution_time;
    int utilization;
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

// Third task
struct hrtimer hr_timer_3_1;
struct hrtimer hr_timer_3_2;


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
            NULL,
            "task1_subtask1", // name
            (int)0,//last release time,assign in runtime
            NUM_ITERS_PER_MS*ONE_1_EXECUTION_TIME,//loop iterations count
            0,//cumulative execution time,calculate later
            ONE_1_EXECUTION_TIME,//execution time
            ONE_1_EXECUTION_TIME * 100 /TASK1_PERIOD,//utilization
            -1,//core, calculate later
            0,//relative deadline, calculate later
            0,//priority, calculate later
            TASK1_INDEX,//task index
            0
        },
        // Subtask 2
        {
            NULL,//hrtimer
            NULL,//task_struct pointer
            NULL,
            "task1_subtask2", // name
            (int)0,//last release time,assign in runtime
            NUM_ITERS_PER_MS*ONE_2_EXECUTION_TIME,//loop iterations count
            0,//cumulative execution time,calculate later
            ONE_2_EXECUTION_TIME,//execution time
            ONE_2_EXECUTION_TIME * 100/TASK1_PERIOD,//utilization
            -1,//core, calculate later
            0,//relative deadline, calculate later
            0,//priority, calculate later
            TASK1_INDEX,//task index
            1
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
            NULL,
            "task2_subtask1", // name
            (int)0,//last release time,assign in runtime
            NUM_ITERS_PER_MS*TWO_1_EXECUTION_TIME,//loop iterations count
            0,//cumulative execution time,calculate later
            TWO_1_EXECUTION_TIME,//execution time
            TWO_1_EXECUTION_TIME*100/TASK2_PERIOD,//utilization
            -1,//core, calculate later
            0,//relative deadline, calculate later
            0,//priority, calculate later
            TASK2_INDEX,//task index
            0
        },
        // Subtask 2
        {
            NULL,//hrtimer
            NULL,//task_struct pointer
            NULL,
            "task2_subtask2", // name
            (int)0,//last release time,assign in runtime
            NUM_ITERS_PER_MS*TWO_2_EXECUTION_TIME,//loop iterations count
            0,//cumulative execution time,calculate later
            TWO_2_EXECUTION_TIME,//execution time
            TWO_2_EXECUTION_TIME*100/TASK2_PERIOD,//utilization
            -1,//core, calculate later
            0,//relative deadline, calculate later
            0,//priority, calculate later
            TASK2_INDEX,//task index
            1
        },
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
            NULL,
            "task3_subtask1", // name
            (int)0,//last release time,assign in runtime
            NUM_ITERS_PER_MS*THREE_1_EXECUTION_TIME,//loop iterations count
            0,//cumulative execution time,calculate later
            THREE_1_EXECUTION_TIME,//execution time
            THREE_1_EXECUTION_TIME*100/TASK3_PERIOD,//utilization
            -1,//core, calculate later
            0,//relative deadline, calculate later
            0,//priority, calculate later
            TASK3_INDEX,//task index
            0
        },
        // Subtask 2
        {
            NULL,//hrtimer
            NULL,//task_struct pointer
            NULL,
            "task3_subtask2", // name
            (int)0,//last release time,assign in runtime
            NUM_ITERS_PER_MS*THREE_2_EXECUTION_TIME,//loop iterations count
            0,//cumulative execution time,calculate later
            THREE_2_EXECUTION_TIME,//execution time
            THREE_2_EXECUTION_TIME*100/TASK3_PERIOD,//utilization
            -1,//core, calculate later
            0,//relative deadline, calculate later
            0,//priority, calculate later
            TASK3_INDEX,//task index
            1
        },
    }
};

#endif
