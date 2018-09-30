/*******************************************************************************
* Copyright (c) 2015 Song Yang @ ittraining
* 
* All rights reserved.
* This program is free to use, but the ban on selling behavior.
* Modify the program must keep all the original text description.
*
* 保留所有權利。
* 本程式可任意使用，但是禁止販售行為。
* 修改程式時必須保留所有原有文字說明。
* 
* Email: onionys@ittraining.com.tw
* Blog : http://blog.ittraining.com.tw
*******************************************************************************/

#include <linux/module.h>   
#include <linux/string.h>    
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>      
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/uaccess.h>

#define MY_MAJOR  200
#define MY_MINOR  0
#define MY_DEV_COUNT 2
#define NAME	"chrnew_mydev"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ITtraining.com.tw");
MODULE_DESCRIPTION("A Simple GPIO Device Driver module for RaspPi");

static int     my_open( struct inode *, struct file * );
static ssize_t my_read( struct file * ,        char *  , size_t, loff_t *);
static ssize_t my_write(struct file * , const  char *  , size_t, loff_t *);
static int     my_close(struct inode *, struct file * );
struct file_operations my_fops = {
        read    :       my_read,
        write   :       my_write,
        open    :       my_open,
        release :       my_close,
        owner   :       THIS_MODULE
};
struct cdev my_cdev;
static char *msg = NULL;

/*
 * INIT_MODULE -- MODULE START --
 * */
int init_module(void)
{
	// -- initial the device number
	dev_t devno;
	unsigned int count = MY_DEV_COUNT;
	int err;
	devno = MKDEV(MY_MAJOR, MY_MINOR);
	register_chrdev_region(devno, count , NAME);

	// -- initial the char device 
	cdev_init(&my_cdev, &my_fops);
	my_cdev.owner = THIS_MODULE;
	err = cdev_add(&my_cdev, devno, count);

	// -- check error of adding char device 
	if (err < 0)
	{
		printk("Device Add Error\n");
		return -1;
	}

	printk("Hello World. This is my first char dev.\n");
	printk("'mknod /dev/mydev_0 c %d 0'.\n", MY_MAJOR);
	printk("'mknod /dev/mydev_1 c %d 1'.\n", MY_MAJOR);

	msg = (char *)kmalloc(32, GFP_KERNEL);
	if (msg !=NULL)
		printk("malloc allocator address: 0x%p\n", msg);

    return 0;
}


/*
 * CLEANUP_MODULE -- MODULE END --
 * */
void cleanup_module(void)
{
	dev_t devno;
	printk("Goodbye\n");

	if (msg){
        /* release the malloc */
        kfree(msg);
	}

	// -- release the char device 
	cdev_del(&my_cdev);
	devno = MKDEV(MY_MAJOR, MY_MINOR);
	unregister_chrdev_region(devno, MY_DEV_COUNT);

}


/*
 * file operation: OPEN 
 * */
static int my_open(struct inode *inod, struct file *fil)
{
	int major;
	int minor;
	major = imajor(inod);
	minor = iminor(inod);
	printk("\n*****Some body is opening me at major %d  minor %d*****\n",major, minor);
	return 0;
}


/*
 * file operation: READ
 * */
static ssize_t my_read(struct file *filp, char *buff, size_t len, loff_t *off)
{
	int major, minor;
	short count;

	major = MAJOR(filp->f_path.dentry->d_inode->i_rdev);
	minor = MINOR(filp->f_path.dentry->d_inode->i_rdev);
	printk("FILE OPERATION READ:%d:%d\n", major, minor);

	switch(minor){
		case 0:
			strcpy(msg,"DATA FROM MOUDLE: minor : 0");
			break;
		case 1:
			strcpy(msg,"DATA FROM MOUDLE: minor : 1");
			break;
		default:
			len = 0;
	}
	count = copy_to_user( buff, msg, len);
	return 0;
}


/*
 * file operation: WRITE
 * */
static ssize_t my_write(struct file *filp, const char *buff, size_t len, loff_t *off)
{
	int major,minor;
	short count;

	memset(msg, 0, 32);
	major = MAJOR(filp->f_path.dentry->d_inode->i_rdev);
	minor = MINOR(filp->f_path.dentry->d_inode->i_rdev);
	// -- copy the string from the user space program which open and write this device
	count = copy_from_user( msg, buff, len );

	printk("FILE OPERATION WRITE:%d:%d\n",major,minor);
	printk("msg: %s", msg);

	return len;
}


/*
 * file operation : CLOSE
 * */
static int my_close(struct inode *inod, struct file *filp)
{
	int minor;
	minor = MINOR(filp->f_path.dentry->d_inode->i_rdev);
	printk("*****Some body is closing me at minor %d*****\n",minor);
	return 0;
}

