#if 1
  #define dprint(fmt,s...) printk("chrauto: %s,%d:"fmt,__FUNCTION__,__LINE__,##s)
#else
  #define dprint(fmt,s...)
#endif  

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <asm/uaccess.h>

#define DEVICE_NAME		"chrauto"
#define NUMBERS			2
#define BUF_SIZE			32

struct my_dev{
	struct cdev cdev;
	char kbuf[BUF_SIZE];
} *pmy_dev;

static struct class  *mydev_class; 
static int major_num,minor_num;	
static ssize_t drv_read(struct file *filp, char *buf, size_t count, loff_t *ppos)
{
	struct my_dev* pdev = filp->private_data;
	int n;
	

	n=strlen(pdev->kbuf);
	if ( copy_to_user(buf, pdev->kbuf,n) ) { 
		dprint("Copy data to user context fail !\n");
		//kfree(kbuf);
		 return -EFAULT;
         }

         dprint("counut=%d, ret=%d\n", count,n);
	return n;
 }

static ssize_t drv_write(struct file *filp, const char *buf, size_t count, loff_t *ppos)
{
	struct my_dev* pdev = filp->private_data;

	if(count <= BUF_SIZE){
	  if( copy_from_user(pdev->kbuf, buf, count) ) {
		  dprint("Get data from user failed !\n");
		  return -EFAULT;
	  }
	}
	else{
	  if( copy_from_user(pdev->kbuf, buf, BUF_SIZE) ) {
		  dprint("Get data from user failed !\n");
		  //kfree(kbuf);
		  return -EFAULT;
	  }	  
	}
	
	pdev->kbuf[count] = 0;
	dprint("Write data : %s\n", pdev->kbuf);
	return count;
}

static int drv_open(struct inode *inode, struct file *filp)
{
	dprint("device open\n");
	
	struct my_dev* pdev = container_of(inode->i_cdev,struct my_dev, cdev);
	filp->private_data = pdev;

	return 0;
}

static long drv_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	dprint("device ioctl\n");
	return 0;
}

static int drv_release(struct inode *inode, struct file *filp)
{
	dprint("device close\n");
	return 0;
}

struct file_operations drv_fops =
{
	.open			=           drv_open,	 
	.read			=           drv_read,
	.write			=           drv_write,
	.unlocked_ioctl		=   	    drv_ioctl,
	.release			=	    drv_release,
};
 

static int demo_init(void) 
{	
	dev_t  mydev_num; 	
	dprint("demo_init for chr_auto \n");
	
	//Kernel assign major & minor number dynamically
	if(alloc_chrdev_region(&mydev_num,0,NUMBERS,DEVICE_NAME) < 0){
		dprint("can't register device number\n");
		return -1;
	}
	
	major_num = MAJOR(mydev_num);
	minor_num = MINOR(mydev_num);
	dprint("Major number = %d\n", major_num);
	dprint("Minor number = %d\n", minor_num);

	// step 1:cdev memory allocate 	
	pmy_dev = kzalloc(sizeof(struct my_dev),GFP_KERNEL);  // Allocate memory for private struct
	if(!pmy_dev)
	{
		dprint("Bad Kmalloc\n");
	}
	
	//step 2:init cdev
	cdev_init(&pmy_dev->cdev,&drv_fops);
	pmy_dev->cdev.owner = THIS_MODULE;
	
	//step 3:cdev_add	
	if(cdev_add(&pmy_dev->cdev,mydev_num,1))
	{
		dprint("Bad cdev\n");
		return 1;
	}

	// step 4:send uevent to udev, create /dev nodes
	mydev_class = class_create(THIS_MODULE,DEVICE_NAME); // create class put in /sysfs
	//create /dev node and connect it to class that put in /sysfs
	device_create(mydev_class,NULL,mydev_num,NULL,DEVICE_NAME);


	dprint("Demo Initialize finished\n");
	return 0;
}

static void demo_exit(void) {
  
	dev_t  mydev_num = MKDEV(major_num,minor_num);	
	
	device_destroy(mydev_class,mydev_num);  
	class_destroy(mydev_class); 
	cdev_del(&pmy_dev->cdev);		
	unregister_chrdev(major_num, DEVICE_NAME);  	
	kfree(pmy_dev);	
	dprint("%s: removed\n", DEVICE_NAME);  	
}

module_init(demo_init);
module_exit(demo_exit);
MODULE_LICENSE("GPL");