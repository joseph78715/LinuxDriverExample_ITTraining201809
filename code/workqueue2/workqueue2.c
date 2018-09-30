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
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
 
MODULE_LICENSE("Dual BSD/GPL");
 
struct work_struct workq;
 
void my_workqueue_handler(struct work_struct *work)
{
	printk("hello work queue: START my_workqueue_handler() \n");
	msleep(3000);  /* sleep */
	printk("hello work queue: END   my_workqueue_handler() \n");
}
 
static int my_init(void)
{
	int ret = 0;
	printk("hello work queue: START module_init() \n");
	INIT_WORK(&workq, my_workqueue_handler);
	schedule_work(&workq);
	printk("hello work queue: END   module_init() \n");
	return 0;
}
 
static void my_exit(void)
{
	flush_scheduled_work();
	cancel_work_sync(&workq);
	printk("my driver removed.\n");
}
 
module_init(my_init);
module_exit(my_exit);
