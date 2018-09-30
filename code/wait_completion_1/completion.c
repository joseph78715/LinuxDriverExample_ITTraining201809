#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/completion.h> //kangta

MODULE_LICENSE("Dual BSD/GPL");

#define SLEEP_TIMEOUT  (6*HZ)
#define WAKEUP_TIMEOUT (5*HZ)
DECLARE_COMPLETION(comp);//kangta

static struct timer_list tickfn;
static wait_queue_head_t wait;
static int condition = 0;

static void sample_timeout(unsigned long arg)
{
        #if 0
	wait_queue_head_t *wq = (wait_queue_head_t *)arg;

	printk("wq %p (%s)\n", wq, __func__);
	condition = 1;
	wake_up(wq);
        #endif 
        complete(&comp);//kangta
}

static int sample_init(void)
{
	long val;
        long val1;//kangta 

	printk("driver loaded: %p\n", &tickfn);

	init_waitqueue_head(&wait);

	init_timer(&tickfn);
	tickfn.function = sample_timeout;
	tickfn.data = (unsigned long)&wait;
	tickfn.expires = jiffies + WAKEUP_TIMEOUT;
	add_timer(&tickfn);

        //-----------------------------------------------------
        val1 = wait_for_completion_timeout(&comp,SLEEP_TIMEOUT);//kangta
        if(val1 == 0)
          printk("Time out \r\n");
        else
          printk("complete event\r\n");         
        //-----------------------------------------------------

        #if 0
	condition = 0;
	val = wait_event_timeout(wait, condition, SLEEP_TIMEOUT);
	if (val == 0) { /* timeout */
		printk("Timeout ocurred. (%s)\n", __func__);
	}
	else {
		printk("WQ done!! (%s)\n", __func__);
	}
	#endif	
	return 0;
}

static void sample_exit(void)
{
	del_timer_sync(&tickfn);

	printk(KERN_ALERT "driver unloaded\n");
}

module_init(sample_init);
module_exit(sample_exit);

