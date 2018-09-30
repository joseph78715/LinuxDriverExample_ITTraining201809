#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/mutex.h>
#include <linux/tty.h>
#include <linux/proc_fs.h>
#include <linux/timer.h>
#include <asm/uaccess.h>
#include <linux/slab.h>

#include "../include/mydebug.h"

#define MY_MAJOR	40

static struct proc_dir_entry	*MyProcEntryFd;
#define MY_PROC_NAME		"driver/myproc"

static ssize_t	my_read_proc(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	char kbuf[] ={"proc read test"};
	int n;
	static bool do_it = false;
	
	n=strlen(kbuf);
  	dprint("counut=%d, kbuf=%d\n", count,n);
	
	if ( copy_to_user(buf, kbuf,n) ) { 
		dprint("Copy data to user context fail !\n");
		return 0;
         }
#if 1         
	if(do_it ){
	  do_it = false;
	  return 0;
	}
	else{
	  do_it = true;
	  return n;
	}
#else
	return n;
	//return 0;	
#endif

}

static int	my_write_proc(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	char	*buf;

	dprint("count=%d\n", count);
	buf = kmalloc(count+1, GFP_KERNEL);
	if ( copy_from_user(buf, (char*)buffer , count) ) {
		kfree(buf);
                return -EFAULT;
	}
	buf[count] = 0;
	dprint("string=%s\n", buf);
	kfree(buf);
	return count;
}

static const struct file_operations my_fops = {
        .owner = THIS_MODULE,
        .read = my_read_proc,
        .write = my_write_proc,
};

static int	__init proc_init(void)
{
	dprint("\n");
#if 0	
	MyProcEntryFd = create_proc_entry(MY_PROC_NAME, 0, NULL);
	MyProcEntryFd->read_proc = my_read_proc;
	MyProcEntryFd->write_proc = my_write_proc;
#endif	
	MyProcEntryFd = proc_create(MY_PROC_NAME, 0,NULL,&my_fops);
	return 0;
}

static void	__exit proc_exit(void)
{
	dprint("\n");
	remove_proc_entry(MY_PROC_NAME, 0);
}

module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GNU");
