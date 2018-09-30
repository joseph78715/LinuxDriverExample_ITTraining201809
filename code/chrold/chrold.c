#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/tty.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#if 1
  #define dprint(fmt,s...) printk("chrold_dev: %s,%d:"fmt,__FUNCTION__,__LINE__,##s)
#else
  #define dprint(fmt,s...)
#endif  

#define MY_MAJOR		40
#define NAME		"chrold_mydev"

static loff_t	my_lseek(struct file *file, loff_t offset, int orig)
{
	dprint("\n");
	return file->f_pos;
}

static ssize_t	my_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	char	*kbuf;
	int n;

        kbuf=file->private_data;
	dprint("counut=%d\n", count);

	n=strlen(kbuf);
	if ( copy_to_user(buf, kbuf,n) ) { 
		dprint("Copy data to user context fail !\n");
		//kfree(kbuf);
		return 0;
         }
         
	return n;
}

static ssize_t	my_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	char	*kbuf=(char *)file->private_data;
	int 	i;

	if ( copy_from_user(kbuf, buf, count) ) {
		dprint("Get data from user failed !\n");
		//kfree(kbuf);
		return 0;
	}
	

	kbuf[count] = 0;
	dprint("Write data : %s\n", kbuf);
//	kfree(kbuf);
	return count;
}

static int	my_open(struct inode *inode, struct file *file)
{
	int	minor=iminor(inode);
          file->private_data = kmalloc(100, GFP_KERNEL);
     if ( file->private_data == NULL ) {
		 printk("Kernel memory allocate failed !\n");
		 return 0;
	  }
	memset(file->private_data,0,100);

	dprint("minor=%d\n", minor);
	return 0;
}

static long	my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	dprint("\n");
	return 0;
}

static int	my_release(struct inode *inode, struct file *file)
{
	dprint("\n");
	kfree(file->private_data);
	return 0;
}

static const struct file_operations	my_fops = {
	.llseek		= my_lseek,
	.read		= my_read,
	.write		= my_write,
	.open		= my_open,
	.unlocked_ioctl	= my_ioctl,
	.release	= my_release,
};

static int	__init ex3_init(void)
{
	dprint(" Regist mydrver,before run test app\n");
	dprint(" Don't forget make inode by hand: mknod /dev/ex3 c 40 0\n");	
	return register_chrdev(MY_MAJOR, NAME, &my_fops);
}

static void	__exit ex3_exit(void)
{
	dprint("Unregist mydrver\n");
	unregister_chrdev(MY_MAJOR, NAME);
}

module_init(ex3_init);
module_exit(ex3_exit);

MODULE_LICENSE("GNU");
