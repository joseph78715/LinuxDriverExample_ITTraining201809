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
#include <linux/interrupt.h>
#include <linux/delay.h> 
MODULE_LICENSE("Dual BSD/GPL");
 
static void my_tasklet_handler(unsigned long flag);
DECLARE_TASKLET(my_tasklet, my_tasklet_handler, 0);
 
static void my_tasklet_handler(unsigned long flag)
{
#if(0)
  	msleep(1);  // ISR fall in sleep,enable this compile will fail
#else
	mdelay(1); // busy loop
#endif
	printk("my_tasklet run: do what the tasklet want to do....\n");

}
 
static int hello_tasklet_init(void)
{
	printk("module init start. \n");
	printk("Hello tasklet!\n");
	tasklet_schedule(&my_tasklet);
	printk("module init end.\n");
	return 0;
}
 
static void hello_tasklet_exit(void)
{
	tasklet_kill(&my_tasklet);
	printk("Goodbye, tasklet!\n");
}
 
module_init(hello_tasklet_init);
module_exit(hello_tasklet_exit);
