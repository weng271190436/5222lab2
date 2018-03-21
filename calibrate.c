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
