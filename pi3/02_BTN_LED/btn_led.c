/*******************************************************************************
* Copyright (c) 2015 Song Yang @ ittraining
*
* All rights reserved.
* This program is free to use, but the ban on selling behavior.
* Modify the program must keep all the original text description.
*
* Email: onionys@ittraining.com.tw
* Blog : http://blog.ittraining.com.tw
*******************************************************************************/


#include <linux/module.h>   
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>


#define LED 5
#define COM 22
#define BUTTON 23
#define MY_GPIO_INT_NAME "my_button_int"
#define MY_DEV_NAME "my_device"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A Simple GPIO Device Driver module for RaspPi");

static short int button_irq = 0;
static unsigned long flags = 0;
static int led_trigger = 0;

static irqreturn_t button_isr(int irq, void *data)
{
	local_irq_save(flags);
	printk("button_isr !!!!\n");
	gpio_set_value(LED, led_trigger);
	led_trigger = led_trigger ? (0):(1);
	local_irq_restore(flags);
	return IRQ_HANDLED;
}


int init_module(void)
{
	// -- setup the led gpio as output
	printk("module: button interrupt example.\n");
	if(gpio_is_valid(LED) < 0) return -1;
	if(gpio_request(LED, "LED") < 0) return -1;
	gpio_direction_output(LED, 0 );

	// -- setup the button gpio as input and request irq
	if(gpio_request(BUTTON,"BUTTON") < 0) return -1;
	if(gpio_is_valid(BUTTON) < 0) return -1;
	if( (button_irq = gpio_to_irq(BUTTON)) < 0 )  return -1;
	if( request_irq( button_irq, button_isr ,IRQF_TRIGGER_RISING, MY_GPIO_INT_NAME, MY_DEV_NAME)) return -1;
        
        if(gpio_is_valid(COM) < 0){
		printk("gpio %d is valid error \n", COM);
		return -1;
	}
	
	if(gpio_request(COM,"COM_GPIO") < 0){
		printk("gpio %d is request error \n", COM);
		return -1;
	}
	
	gpio_direction_output(COM, 1);        
        return 0;
	return 0;

}


void cleanup_module(void)
{
	gpio_set_value(LED, 0);
	gpio_free(LED);
	free_irq(button_irq, MY_DEV_NAME);
	gpio_free(BUTTON);
	gpio_free(COM);        
}
