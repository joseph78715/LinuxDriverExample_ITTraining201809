/*
 * main.c
 *
 */
#include <linux/init.h>
#include <linux/wait.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/sched.h>

MODULE_LICENSE("Dual BSD/GPL");

static struct task_struct *kmain_task = NULL;
static wait_queue_head_t waitq;

static int sample_thread(void *num)
{
	printk("%s called\n", __func__);
	
	DEFINE_WAIT(wait);
	for (;;) {
#if 0
	  //this function has been removed from kernel 3.15
	  interruptible_sleep_on_timeout(&waitq, 3 * HZ);
#else
	  prepare_to_wait(&waitq,&wait,TASK_INTERRUPTIBLE);
	  schedule_timeout(3 * HZ);
	  finish_wait(&waitq,&wait);
#endif	  
      
	    printk("%s called\n", __func__);
		if (kthread_should_stop())
			break;
	}

	printk("%s leaved\n", __func__);

	return 0;
}

static int sample_init(void)
{
	printk("sample driver installed.\n");

	init_waitqueue_head(&waitq);

	kmain_task = kthread_create(sample_thread, NULL, "sample_kthread");
	if (IS_ERR(kmain_task)) {
		return PTR_ERR(kmain_task);
	}
	wake_up_process(kmain_task);

	return 0;
}

static void sample_exit(void)
{
	printk("sample driver removed.\n");

	kthread_stop(kmain_task);
}

module_init(sample_init);
module_exit(sample_exit);

