/*******************************************************************************
* Copyright (c) 2015 Song Yang @ ittraining
* 
* All rights reserved.
* This program is free to use, but the ban on selling behavior.
* Modify the program must keep all the original text description.
*
* 保留所有權利。
* 本程式可任意使用，但是禁止販售行為。
* 修改程式時必須保留所有原有文字說明。
* 
* Email: onionys@ittraining.com.tw
* Blog : http://blog.ittraining.com.tw
*******************************************************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/delay.h>
#include<linux/workqueue.h>
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A Simple Blocking IO device RaspPi");

/* declare a wait queue*/
static wait_queue_head_t my_wait_queue;
 
/* declare a work queue*/
struct work_struct workq;
void my_workqueue_handler(struct work_struct *work)
{
	printk("WORK QUEUE: I'm just a timer to wake up the sleeping moudlue. \n");
	msleep(10000);  /* sleep 10 sec*/
	printk("WORK QUEUE: time up MODULE !! wake up !!!! \n");

	wake_up_interruptible(&my_wait_queue);
}
 
/*
 * INIT_MODULE -- MODULE START --
 * */
int init_module(void)
{
	printk("Wait queue example ....\n");
 
	// -- initialize the work queue
	INIT_WORK(&workq, my_workqueue_handler);
	schedule_work(&workq);
 	msleep(1000);  // sleep 1 sec
	// -- initialize the WAIT QUEUE head
	init_waitqueue_head(& my_wait_queue);
	
	DEFINE_WAIT(wait);	
 
	printk("MODULE: This moudle is goint to sleep....\n");
#if 0
	  //this function has been removed from this kernel version
	interruptible_sleep_on(&my_wait_queue);
#else
	prepare_to_wait(&my_wait_queue,&wait,TASK_INTERRUPTIBLE);
	schedule();
#endif

	printk("MODULE: Wakeup Wakeup I am Waked up........\n");
	return 0;
}
 
/*
 * CLEANUP_MODULE -- MODULE END --
 * */
void cleanup_module(void)
{
	printk("<1> Start to cleanup \n");
}
