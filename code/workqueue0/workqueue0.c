/*
 * History:
 * Date		Aurhor			Comment
 */
#include <linux/proc_fs.h>
#include <linux/unistd.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/mm.h>
#include <linux/kmod.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <asm/io.h>
#include <asm/uaccess.h>
//#include <asm/system.h>
#include <asm/irq.h>
#include <asm/bitops.h>

#define VICTOR_DEBUG
#include "../include/mydebug.h"

struct my_struct {
	int			a;
	struct work_struct	workqueue;
	int			b;
} mydata;

static void workqueue_run(struct work_struct *work)
{
	struct my_struct	*ptr;

	ptr = container_of(work, struct my_struct, workqueue);
	
	dprint("get mydata address = 0x%p,a=%d\n", ptr,ptr->a);
}

static void __exit workqueue_exit(void)
{
	dprint("\n");
	cancel_work_sync(&mydata.workqueue);
//	flush_work(&mydata.workqueue);
}

static int __init workqueue_init(void)
{
	dprint("mydata address = 0x%p\n", &mydata);
	mydata.a=10;
	INIT_WORK(&mydata.workqueue, workqueue_run);
	schedule_work(&mydata.workqueue);
	return 0;
}

module_init(workqueue_init);
module_exit(workqueue_exit);

MODULE_AUTHOR("Hello");
MODULE_LICENSE("GPL");
