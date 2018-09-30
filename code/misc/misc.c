/*
 * History:
 * Date		Aurhor			Comment
 */
#include <linux/proc_fs.h>
#include <linux/unistd.h>
#include <linux/string.h>
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/spinlock.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/mm.h>
#include <linux/kmod.h>
#include <linux/platform_device.h>
#include <linux/workqueue.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/bitops.h>
#include <linux/slab.h>

#if 1
  #define dbg_printk(fmt,s...) printk("=mycode=: %s,%d:"fmt,__FUNCTION__,__LINE__,##s)
#else
  #define dbg_printk(fmt,s...)
#endif  

#define VICTOR_MISC_MINOR	106
static int			opencounts=0;

static ssize_t  misc_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
        dbg_printk("\n");
        return 0;
}

static ssize_t  misc_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char	*ptr;

        dbg_printk("count=%d\n", count);
	ptr = kmalloc(count+1, GFP_KERNEL);
        if ( copy_from_user(ptr, (char*)buf , count) ) {
                kfree(ptr);
                return -EFAULT;
        }
        ptr[count] = 0;
        dbg_printk("string=%s\n", ptr);
        kfree(ptr);
        return count;
}

static int misc_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	switch ( cmd ) {
	case 1 :
		dbg_printk("\n");
		break;
	case 2 :
		dbg_printk("\n");
		break;
	case 3 :
		dbg_printk("\n");
		break;
	case 4 :
		dbg_printk("\n");
		break;
	default:
		dbg_printk("\n");
		return -EINVAL;
	}
	return 0;
}

static int misc_open(struct inode *inode, struct file *file)
{
	unsigned long	flags;

	if ( MINOR(inode->i_rdev) != VICTOR_MISC_MINOR )
		return -ENODEV;
	dbg_printk("major=%d,minor=%d\n", imajor(inode), iminor(inode));
	local_irq_save(flags);
	opencounts++;
	local_irq_restore(flags);
	dbg_printk("opencounts=%d\n", opencounts);
	return 0;
}

static int misc_release(struct inode *inode, struct file *file)
{
	unsigned long	flags;

	local_irq_save(flags);
	opencounts--;
	local_irq_restore(flags);
	dbg_printk("opencounts=%d\n", opencounts);
	return 0;
}

static struct file_operations misc_fops = {
	owner:THIS_MODULE,
	llseek:NULL,
	compat_ioctl:misc_ioctl,
	read:misc_read,
	write:misc_write,
	open:misc_open,
	release:misc_release,
};
static struct miscdevice misc_dev = {
	VICTOR_MISC_MINOR,
	"vmisc",
	&misc_fops
};

static void __exit victor_misc_exit(void)
{
	misc_deregister(&misc_dev);
}

static int __init victor_misc_init(void)
{
	// register misc
	if ( misc_register(&misc_dev) ) {
		printk("Register misc fail !\n");
		return -1;
	}

	return 0;
}

module_init(victor_misc_init);
module_exit(victor_misc_exit);

MODULE_AUTHOR("Hello");
MODULE_LICENSE("GPL");
