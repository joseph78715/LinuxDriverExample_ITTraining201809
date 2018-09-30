#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("Dual BSD/GPL");



static void __exit hello_exit(void)
{
    printk("Goodbye, hello world\n");
}

static int __init hello_init(void)
{
    printk("Hello world!\n");
	while(1);
    //hello_exit();
    return 0;
}

module_init(hello_init);
module_exit(hello_exit);

