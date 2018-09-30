
#include <linux/input.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define GPIO_BTN			23
#define GPIO_BTN_DESC		"pi2_btn"
#define GPIO_BTN_DESC_DEV	"my_btn"

#define GPIO_LED			22
#define GPIO_LED_DESC		"pi2_led"

#define NAME			"PI2_BUTTON"

static struct input_dev *button_dev = NULL;
static short int button_irq = 0;

static irqreturn_t button_interrupt(int irq, void *dummy)
{
	int value = gpio_get_value(GPIO_BTN);
	gpio_set_value(GPIO_LED, value ? 0:1);
	input_report_key(button_dev, BTN_0, value ? 0:1);	//send to input core
	input_sync(button_dev);

	return IRQ_HANDLED;
}

static int initial_gpio_set(void)
{
	//request GPIO of button
	if(gpio_request(GPIO_BTN, GPIO_BTN_DESC) < 0) return -1;
	if(gpio_is_valid(GPIO_BTN) < 0) return -1;
	if( (button_irq = gpio_to_irq(GPIO_BTN)) < 0 )  return -1;
	
	if (request_irq( button_irq, 
			(irq_handler_t) button_interrupt,
			IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, 
			GPIO_BTN_DESC,
			GPIO_BTN_DESC_DEV)) {
                printk("Can't allocate irq %d\n", button_irq);
                return -EBUSY;
        }
        
        // request GPIO of LED
        if(gpio_request(GPIO_LED, GPIO_LED_DESC) < 0) return -1;
	if(gpio_is_valid(GPIO_LED) < 0) return -1;        
        gpio_direction_output(GPIO_LED,0);
	
	return 0;
}

static int __init button_init(void)
{
	int error;

	button_dev = input_allocate_device();
	if (!button_dev) {
		printk("Not enough memory\n");
		error = -ENOMEM;
		goto err_free_irq;
	}
	
	button_dev->name = NAME;		
	// Two ways for setting, first way is simple and second way is suitable for multiple keys with logical OR
#if 1
	set_bit(EV_KEY,button_dev->evbit);
	set_bit(BTN_0,button_dev->keybit);
#else
	button_dev->evbit[0] = BIT_MASK(EV_KEY);
	button_dev->keybit[BIT_WORD(BTN_0)] = BIT_MASK(BTN_0);
#endif
	
	error = input_register_device(button_dev);
	if (error) {
		printk("Failed to register device\n");
		goto err_free_dev;
	}

	error = initial_gpio_set();
	if(error < 0){
	    printk("initial gpio set fail\n");
	    goto err_free_dev;
	}
	
	return 0;

 err_free_dev:
	input_free_device(button_dev);
 err_free_irq:
	free_irq(button_irq, GPIO_BTN_DESC_DEV);
	return error;
}

static void __exit button_exit(void)
{

	input_unregister_device(button_dev);
	free_irq(button_irq,GPIO_BTN_DESC_DEV);
	gpio_free(GPIO_BTN);
	gpio_free(GPIO_LED);	
	printk("input_device exit \n");	
}

module_init(button_init);
module_exit(button_exit);
MODULE_LICENSE("GPL");
