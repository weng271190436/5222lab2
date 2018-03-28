# CSE 522 Lab 2
## Zimu Wang zimu.wang@wustl.edu
## Wei Weng wengwei@wustl.edu
## Diqiu Zhou zhoudiqiu@wustl.edu


# Lab design:

## Layout of the data structure
As in step 4, the struct subtask contains everything the kernel module needs to know about this subtask, including task_struct pointer, hrtimer pointer, mother task pointer etc. All the rest of the fields, such as utilization, execution time are all in integer because it seems like it's unsafe to use float in kernel space. With the struct, we can pass the subtask struct itself to the thread functions without the need to design additional structures that stores the attributes.

## Static vs. Dynamic initialization
The lab instruction recommends using static initialization for all the task and subtask structs in the header file, and then
use those directly with careful indexing in the kernel module. When writing the lab, we felt rather restricted by the purely static approach. Certain fields in the subtask structs could be better calculated dynamically. For example, the calibrate.c
should be able to take in a list of task of arbitrary size, and perform the action accordingly. We initially wanted to declare all the lists of tasks and subtasks in the header file, and use extern keyword to make it visible to the kernel module. This approach led to some unknown symbol problem that we weren't able to fix. Instead, we moved every such structure and initialize and cleanup function to the main calibrate.c function, which also increased readability of the code.

## Initialize function
The key in initialization is creating two lists. The first list is the list of tasks that holds the subtasks. The second list is
a list (or actually 4 lists) containing the subtasks assigned to each core. During initialization, each subtask gets its hrtimer struct initialized, and fields such as cumulative_execution_time and execution_time calculated. We decided to calculate all the required fields in a single initialization step to make other parts of the code cleaner.


## Calibration mode design:

I set the initial loop iterations count of every subtask to be 1000.
I use an incrementor with the initial value of 2^14 and try to add the incrementor to the loop iterations count.
If the incrementor is too large, I divide the incrementor by 2 and try again.
If the incrementor is small enough so that the sum of the incrementor and the loop iterations count does not
run longer than the designated execution time, I add the incrementor to the loop iterations count.
I repeat the above process until incrementor becomes 0.

This implementation is easier than the binary search approach. I was trying to implement a simple binary search
for the loop iterations count, by setting the lower bound to 0 and the upper bound to INT_MAX. The running time
is too long. Therefore, I chose this implementation. Generally, this implementation is fast enough for our purpose.

I chose the initial loop iterations count to be 1000 because this is smaller than the loop iterations count for 1ms
of execution time, so I think this is small enough. I chose the incrementor to be 2^14 because this number is in the
order of the loop iterations count for 1ms of execution time, so I think this is large enough.

## Most of the rest of the code:
Most of the remaining code are relatively straightforward and are mostly following the instructions. Because we allocated memory for the hrtimer struct and core_list struct during initialization, we have to make sure those memory are properly freed when the module exits.

## Problems
We decided to use zero length array in both the struct task and struct core_list to be able to deal with variable length of subtasks. The get_parent_task function was not calculating the correct mother task that holds the list of subtasks because, I guess, the struct is not aligned correctly. What is the default memory layout for a zero length array? I thought that array is a continuous piece of memory where each item is stored directly next to each other.
