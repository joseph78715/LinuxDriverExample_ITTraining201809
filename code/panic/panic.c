#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("Dual BSD/GPL");

static int panic_init(void)
{
    panic(" test \n");
    printk("panic happen!\n");
    return 0;
}

static void panic_exit(void)
{
    printk("Goodbye, panic can not execut to here\n");
}

module_init(panic_init);
module_exit(panic_exit);

