
#include <linux/module.h>
#include "../include/mydebug.h"

static unsigned int max = 5;
module_param(max, uint, 0);
MODULE_PARM_DESC(max, "Maximum bytes per I/O (default 5)");



static int	__init hello_init(void)
{
	dprint("max=%d\n",max);
	return 0;
}

static void	__exit hello_exit(void)
{
	dprint("\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GNU");
