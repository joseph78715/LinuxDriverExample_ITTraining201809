#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("Dual BSD/GPL");

static int woops_init(void)
{
    *(int*)0 = 1;
    printk("Hello woops!\n");
    return 0;
}

static void woops_exit(void)
{
    printk("Goodbye, hello woops\n");
}

module_init(woops_init);
module_exit(woops_exit);

