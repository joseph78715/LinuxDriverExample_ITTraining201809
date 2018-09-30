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
#include <linux/timer.h>
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A Simple GPIO Device Driver module for RaspPi");
 
struct timer_list my_timer;
 
/*
 * TIMER FUNCTION
 * */
 
static void timer_function(unsigned long data){
	printk("Time's up\n");
	// modify the timer for next time
	mod_timer(&my_timer, jiffies + HZ / 2);
}
 
/*
 * INIT MODULE
 * */
int init_module(void)
{
	printk("Hello My Timer\n");
 
	//  -- initialize the timer
	init_timer(&my_timer);
	my_timer.expires = jiffies + HZ ;
	my_timer.function = timer_function;
	my_timer.data = NULL;
 
	// -- TIMER START
	add_timer(&my_timer);
	printk("END: init_module() \n");
	return 0;
}
 
/*
 * CLEANUP MODULE
 * */
void cleanup_module(void)
{
	del_timer(&my_timer);
	printk("Goodbye\n");
}
