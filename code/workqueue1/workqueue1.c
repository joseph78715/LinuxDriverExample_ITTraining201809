/*
 * workqueue sample code
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/delay.h>

MODULE_LICENSE("Dual BSD/GPL");

const int IRQ_NUM = 22; // 17 already cooupy check with "cat /proc/interrupts"
void *irq_dev_id = (void *)&IRQ_NUM;
struct work_struct workq;

void sample_workqueue(struct work_struct *work)
{
	printk("%s called (%ld, %ld, %ld)\n", __func__, 
			in_irq(), in_softirq(), in_interrupt());

	msleep(3000);  /* sleep */
}

irqreturn_t sample_isr(int irq, void *dev_instance)
{
    if(&dev_instance == IRQ_NUM)
    {
	if (printk_ratelimit()) {
		printk("%s: irq %d (%ld, %ld, %ld)\n", __func__, irq,
				in_irq(), in_softirq(), in_interrupt());

		schedule_work(&workq);
	}
    }
	
	return IRQ_NONE;
}

static int sample_init(void)
{
	int ret = 0;

	printk("sample driver installed.\n");

	INIT_WORK(&workq, sample_workqueue);

	ret = request_irq(IRQ_NUM, sample_isr, IRQF_TRIGGER_RISING, "sample", irq_dev_id);
	if (ret) {
		printk("request_irq() failed (%d)\n", ret);
		goto out;
	}

out:
	return (ret);
}

static void sample_exit(void)
{
	flush_scheduled_work();
	free_irq(IRQ_NUM, irq_dev_id);

	printk("sample driver removed.\n");
}

module_init(sample_init);
module_exit(sample_exit);

