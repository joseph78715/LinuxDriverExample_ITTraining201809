// Simple Character Device Driver Module for Raspberry Pi.

#include <linux/module.h>   
#include <linux/string.h>    
#include <linux/fs.h>      
#include <asm/uaccess.h>
#include <linux/init.h>
#include <linux/cdev.h>

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/io.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

#include <linux/list.h>
#include <linux/irq.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
//#include <mach/platform.h>
//#include <mach/gpio.h>
#include <linux/time.h>
#include <linux/delay.h>

#include "../platdata.h"

/* *********************************************
 * PLATFORM DRIVER 
 * location : arch/driver/
 * *********************************************
 * */

// -- PROBE 
static int MY_DUMMY_probe(struct platform_device *pdev)
{
	struct resource *regs;
	struct clk_pdata *p = (struct clk_pdata*)pdev->dev.platform_data;

        int irq ;//= -1;
	printk("my example:The platform name is match, call probe \n");
	regs = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!regs) {
		dev_err(&pdev->dev, "could not get IO memory\n");
		return -ENXIO;
	} 

       printk("resource : regs.start=%#x,regs.end=%#x\n",regs->start,regs->end);
       
	irq = platform_get_irq(pdev, 0);
	if (irq < 0) {
		dev_err(&pdev->dev, "could not get IRQ\n");
		return irq;
	}
	
        printk("resource : irq=%d , pdev->id=%d\n",irq,pdev->id);

 printk("resource : num_clock=%d\n", p->num_clock);
 



	return 0;
}

// -- REMOVE
static int MY_DUMMY_remove(struct platform_device *pdev)
{
	return 0;
}

// -- PLATFORM DRIVER 
static struct platform_driver MY_DUMMY_driver = {
	.driver		= {
		.name	= MY_DUMMY_PLATFORM_DEV_NAME,
		.owner	= THIS_MODULE,
	},
	.probe		= MY_DUMMY_probe,
	.remove		= MY_DUMMY_remove,
};


// -- Module INIT
static int __init MY_DUMMY_init(void)
{
	return platform_driver_register(&MY_DUMMY_driver);
	//return platform_driver_probe(&MY_DUMMY_driver, MY_DUMMY_probe);
}

static void __exit MY_DUMMY_exit(void)
{
	platform_driver_unregister(&MY_DUMMY_driver);
}

module_init(MY_DUMMY_init);
module_exit(MY_DUMMY_exit);

MODULE_DESCRIPTION("Platform device and driver example");
MODULE_AUTHOR("ITTraining");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:" MY_DUMMY_PLATFORM_DEV_NAME);
