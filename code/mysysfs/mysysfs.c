
#include <linux/module.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/err.h>

static struct class *my_class;
static struct device *my_device;

static ssize_t devicetest_show(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	return sprintf(buf, "devicetest show\n");
}

static ssize_t devicetest_store(struct device *dev, struct device_attribute *attr,
			    const char *buf, size_t size)
{
	printk("devicetest get string = %s\n",buf);
	return size;
}

static struct device_attribute sysfs_attrs[] = {
	__ATTR(mytest ,0644 ,devicetest_show,devicetest_store),
	{},
};

static ssize_t abc_show(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	return sprintf(buf, "abc enable show\n");
}

static ssize_t abc_store(struct device *dev, struct device_attribute *attr,
			    const char *buf, size_t size)
{
	printk("abc get string = %s\n",buf);
	return size;
}
static DEVICE_ATTR_RW(abc);

static ssize_t test_show(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	return sprintf(buf, "test enable show\n");
}

static ssize_t test_store(struct device *dev, struct device_attribute *attr,
			    const char *buf, size_t size)
{
	printk("test get string = %s\n",buf);
	return size;
}
static DEVICE_ATTR_RW(test);

static struct attribute *my_attrs[] = {
	&dev_attr_abc.attr,
	&dev_attr_test.attr,	
	NULL,
};
ATTRIBUTE_GROUPS(my);

static int create_timed_output_class(void)
{
	if (!my_class) {
		my_class = class_create(THIS_MODULE, "mysysfs");
		if (IS_ERR(my_class))
			return PTR_ERR(my_class);

		my_class->dev_groups = my_groups; // one way to add files
		
		my_device = device_create(my_class,NULL,MKDEV(200,0),NULL,"mysysfs0");
		
		device_create_file(my_device,&sysfs_attrs); // another way to add files
	}

	return 0;
}


static int __init timed_output_init(void)
{
	printk("create sysfs init\n");
	return create_timed_output_class();
}

static void __exit timed_output_exit(void)
{
	printk("exit sysfs \n");
	device_destroy(my_class,MKDEV(200,0));
	class_destroy(my_class);
}

module_init(timed_output_init);
module_exit(timed_output_exit);

MODULE_AUTHOR("Mike Lockwood <lockwood@android.com>");
MODULE_DESCRIPTION("timed output class driver");
MODULE_LICENSE("GPL");