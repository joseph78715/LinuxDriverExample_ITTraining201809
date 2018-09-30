/*
 * History:
 * Date		Aurhor			Comment
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fcntl.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/kmod.h>
#include <linux/workqueue.h>
#include <linux/kthread.h>

//#define VICTOR_DEBUG
#include "../include/mydebug.h"

#define TIMER_INTERVAL		(3*HZ)	// 3 seconds
static struct timer_list        polltimer;
static struct task_struct	*waitkthread;
static wait_queue_head_t	waitqueue;
static int			exitflag=0;

static int	waitqueue_kthread(void *data)
{
	dprint("\n");
	DEFINE_WAIT(wait);		
	
	while ( exitflag == 0 ) {
	  
#if 0
	//this function has been removed from this kernel version
	interruptible_sleep_on(&my_wait_queue);
#else
	prepare_to_wait(&waitqueue,&wait,TASK_INTERRUPTIBLE);
	schedule();
	dprint("%s be waken up \n",__func__);
#endif	   
	}
	dprint("\n");
	return 0;
}

static void     my_poll(unsigned long noused)
{
        dprint("\n");
	if ( exitflag )
		return;
	wake_up_interruptible(&waitqueue);
        polltimer.expires = jiffies + TIMER_INTERVAL;
        add_timer(&polltimer);
}

static void __exit waitqueue_exit(void)
{
	dprint("\n");
	exitflag = 1;
	del_timer(&polltimer);
	wake_up_interruptible(&waitqueue);
	kthread_stop(waitkthread);
}

static int __init waitqueue_init(void)
{

	init_waitqueue_head(&waitqueue);

	waitkthread = kthread_run(waitqueue_kthread, NULL, "it-waitqueue");
	if ( IS_ERR(waitkthread) ) {
		printk("%s,%d : Create kernel thread fail !\n", __FUNCTION__, __LINE__);
		return -1;
	}
	dprint("\n");

        // initialize polling timer
        init_timer(&polltimer);
        polltimer.function = my_poll;
        polltimer.expires = jiffies + TIMER_INTERVAL;
        add_timer(&polltimer);

	return 0;
}

module_init(waitqueue_init);
module_exit(waitqueue_exit);

MODULE_AUTHOR("Ittraining");
MODULE_LICENSE("GPL");
