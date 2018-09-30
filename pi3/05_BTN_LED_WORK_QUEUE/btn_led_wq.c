#include <linux/module.h>   
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/workqueue.h>

#define LED0 5 
#define LED1 6
#define COM 22
#define BUTTON 23 

#define MY_MAJOR	200
#define MY_MINOR	0

#define MY_INT_NAME "MyInterrupt"
#define MY_DEV_NAME "mydevice"


short int button_irq_num = 0;
unsigned long flags;
int led_trigger = 0;

static void my_work_handler(struct work_struct * work);

DECLARE_WORK(my_work_queue, my_work_handler);
static void my_work_handler(struct work_struct * work)
{
	gpio_set_value(LED0, led_trigger);
	gpio_set_value(LED1, led_trigger);
	led_trigger = led_trigger ? (0):(1);
}


static irqreturn_t r_irq_handler(int irq, void *data) 
{
   local_irq_save(flags);
   schedule_work(&my_work_queue);
   local_irq_restore(flags);
   return IRQ_HANDLED;
}


int init_module(void)
{
	int err;
	
	printk("*****LED GPIO Init ******************\n");
	if(gpio_request(LED0, "LED0") ) return -1;
	if(gpio_request(LED1, "LED1") ) return -1;
	if(gpio_request(BUTTON, "BUTTON") ) return -1;
	gpio_direction_output(LED0, 0);
	gpio_direction_output(LED1, 0);
	gpio_direction_output(COM, 1);
	button_irq_num = gpio_to_irq(BUTTON);
	if(button_irq_num < 0) return -1;
	err = request_irq( button_irq_num, 
			r_irq_handler, 
			IRQF_TRIGGER_RISING, // release BUTTON trigger IRQ
			MY_INT_NAME, 
			MY_DEV_NAME);
	if(err ) return -1;

	return 0;
}

void cleanup_module(void)
{
	free_irq(button_irq_num, MY_DEV_NAME);
	gpio_free(BUTTON);

	gpio_set_value(LED0, 0);
	gpio_set_value(LED1, 0);
	gpio_free(LED0);
	gpio_free(LED1);
	gpio_free(COM);        
	flush_scheduled_work();
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A Simple GPIO Device Driver module for RaspPi");
