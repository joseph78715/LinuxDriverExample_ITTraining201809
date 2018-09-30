#include <linux/fs.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/init.h>
#define  MSG(string, args...)  printk("vmouse: " string, ##args);
/*
 * check the device mapping to event   cat /proc/bus/input/devices 
 * attach gpm to the event 	gpm -m /dev/input/eventX -t evdev
 */
struct input_dev *vms_input_dev;
struct platform_device *vms_plat_dev;

ssize_t vms_store(struct device *dev, struct device_attribute *attr,
				  const char *buffer, size_t count)
{
	int x, y;
	sscanf(buffer, "%d%d", &x, &y);
	printk("vms_store: REL_X = %d , REL_Y = %d \n",x,y);
	input_report_rel(vms_input_dev, REL_X, x); //Generates a REL_X event in the X direction
	input_report_rel(vms_input_dev, REL_Y, y);
	input_sync(vms_input_dev);	//indicates the event is complete
	/*  now the input subsystem collects these two events into single evdev package and sends it out of the door
		throught /dev/input/eventX, where X is the interface number assigned to vms driver
	*/	
	return 0;
}
/* DEVICE_ATTR(_name, _mode, _show, _store), for sysfs */
DEVICE_ATTR(coordinates, 0644, NULL, vms_store);

struct attribute *vms_attrs[] = {
	&dev_attr_coordinates.attr,
	NULL
};

struct attribute_group vms_attr_group = {
	.attrs = vms_attrs,
};

int __init vms_init(void)
{
	// allocate an input_dev
	vms_plat_dev = platform_device_register_simple("vms", -1, NULL, 0); 
	if(IS_ERR(vms_plat_dev))
	{
		PTR_ERR(vms_plat_dev);
		MSG("init: reg platform driver err\n");
	}
	/* create sysfs node to read simulated coordinates */
	sysfs_create_group(&vms_plat_dev->dev.kobj, &vms_attr_group);
	
	vms_input_dev = input_allocate_device();
	if(!vms_input_dev)
	{
		MSG("init: alloc input dev err\n");
	}
	/* set which events that input_dev accept 
		keybit : keys and buttons , set_bit(KEY_UP,vms_input_dev->keybit)
		relbit : relative axes
		absbit : absolute axes
		mscbit : misc events
		ledbit : LEDs
		sndbit : sounds
		ffbit  : force feedback event 
	*/
	vms_input_dev->name = "myMouse";	
#if 0	
	set_bit(EV_REL, vms_input_dev->evbit); /*capture relative axes event*/
	set_bit(REL_X, vms_input_dev->relbit); /*produce axe 'X' movement*/
	set_bit(REL_Y, vms_input_dev->relbit); /*produce axe 'Y' movement*/
#else	  
	/* below setting used in ubuntu 12.04 */
	vms_input_dev->phys = "vmd/input0"; // "vmd" is the driver's name
	vms_input_dev->id.bustype = BUS_VIRTUAL;
	vms_input_dev->id.vendor  = 0x0000;
	vms_input_dev->id.product = 0x0000;
	vms_input_dev->id.version = 0x0000;

	vms_input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
	
	vms_input_dev->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) \
			  | BIT_MASK(BTN_RIGHT) | BIT_MASK(BTN_MIDDLE);
			  
	vms_input_dev->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
	
	vms_input_dev->keybit[BIT_WORD(BTN_MOUSE)] |= BIT_MASK(BTN_SIDE) \
			  | BIT_MASK(BTN_EXTRA);
			  
	vms_input_dev->relbit[0] |= BIT_MASK(REL_WHEEL);	
#endif		
	input_register_device(vms_input_dev);
	
	return 0;
}
module_init(vms_init);

void __exit vms_exit(void)
{
	input_unregister_device(vms_input_dev);
	sysfs_remove_group(&vms_plat_dev->dev.kobj, &vms_attr_group);
	platform_device_unregister(vms_plat_dev);
}
module_exit(vms_exit);
MODULE_LICENSE("GPL");



