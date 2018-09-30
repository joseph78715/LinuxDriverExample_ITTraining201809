/*
 * tasklet sample code
 *
 */
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

MODULE_LICENSE("Dual BSD/GPL");

const int BTN_NUM = 23;
void *irq_dev_id = (void *)&BTN_NUM;
struct tasklet_struct tasklet;

void sample_tasklet(unsigned long data)
{
	printk("%s called (%ld, %ld, %ld)\n", __func__,
			in_irq(), in_softirq(), in_interrupt());
}

irqreturn_t sample_isr(int irq, void *dev_instance)
{
	printk("in the button ISR\n");
	if (printk_ratelimit()) {
		printk("%s: irq %d (%ld, %ld, %ld)\n", __func__, irq,
				in_irq(), in_softirq(), in_interrupt());
		tasklet_schedule(&tasklet);
	}
	
	return IRQ_NONE;
}

static int sample_init(void)
{
	int ret = 0;

	printk("sample driver installed.\n");

	tasklet_init(&tasklet, sample_tasklet, 0);

	if(gpio_is_valid(BTN_NUM) < 0){
		printk("gpio %d is valid error \n", BTN_NUM);
		return -1;
	}

	if(gpio_request(BTN_NUM,"BTN_GPIO") < 0){
		printk("gpio %d is request error \n", BTN_NUM);
		return -1;
	}
	
	if (gpio_direction_input(BTN_NUM) < 0){
		printk("set gpio %D as input fail \n",BTN_NUM);
	}
	
	ret = request_irq(gpio_to_irq(BTN_NUM), sample_isr, IRQF_TRIGGER_RISING, "sample", irq_dev_id);
	if (ret) {
		printk("request_irq() failed (%d)\n", ret);
		tasklet_kill(&tasklet);
		goto out;
	}

out:
	return (ret);
}

static void sample_exit(void)
{
	printk("sample driver removed.\n");

	tasklet_kill(&tasklet);
	free_irq(gpio_to_irq(BTN_NUM), irq_dev_id);
	gpio_free(BTN_NUM);
}

module_init(sample_init);
module_exit(sample_exit);

