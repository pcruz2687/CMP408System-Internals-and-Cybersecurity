#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

//#include <linux/version.h>

#include "piio_driver.h"

static dev_t dev; // Global var for the dev device number
static struct cdev char_dev; // Global var for the char dev structure
static struct class *class_name;
static struct device *device_name;

lkm_data lkmdata;
gpio_pin gpiopin;

static int device_open(struct inode *inode, struct file *file) {
	printk(KERN_INFO "PiIO Driver: Opened()\n");

	try_module_get(THIS_MODULE);
	return 0;
}

static int device_release(struct inode *inode, struct file *file) {
	printk(KERN_INFO "PiIO Driver: Released()\n");

	module_put(THIS_MODULE);
	return 0;
}

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
	switch(cmd) {
		case IOCTL_PIIO_GPIO_GREEN_LED_WRITE:
			copy_from_user(&gpiopin, (gpio_pin *)arg, sizeof(gpio_pin));
			gpio_request(gpiopin.green_pin, gpiopin.desc);
			gpio_direction_output(gpiopin.green_pin, 0);
			gpio_set_value(gpiopin.green_pin, gpiopin.green_value);
			printk("PiIO Driver: Green LED pin %u - value %i\n", gpiopin.green_pin, gpiopin.green_value);
			break;
		case IOCTL_PIIO_GPIO_RED_LED_WRITE:
			copy_from_user(&gpiopin, (gpio_pin *)arg, sizeof(gpio_pin));
			gpio_request(gpiopin.red_pin, gpiopin.desc);
			gpio_direction_output(gpiopin.red_pin, 0);
			gpio_set_value(gpiopin.red_pin, gpiopin.red_value);
			printk("PiIO Driver: Red LED pin %u - value %i\n", gpiopin.red_pin, gpiopin.red_value);
			break;
		default:
			printk("PiIO Driver: Command Format Error\n");
	}

	return 0;
}

struct file_operations Fops = {
	.open = device_open,
	.release = device_release,
	.unlocked_ioctl = device_ioctl,
};

static int __init piio_driver_init(void) {
	int ret_val;
	ret_val = 0;

	if(alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0) {
		printk(KERN_INFO "Cannot allocate major number for device 1\n");
		return -1;
	}
	printk(KERN_INFO "Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
	printk(KERN_INFO "PiIO LKM Inserted Successfully\n");

	class_name = class_create(THIS_MODULE, CLASS_NAME);
	if(IS_ERR(class_name)) {
		unregister_chrdev_region(dev, 1);
		printk(KERN_INFO "Failed to Create Device Class\n");
		return PTR_ERR(class_name);
	}
	printk(KERN_INFO "Device Class Created Successfully");

	device_name = device_create(class_name, NULL, dev, NULL, DEVICE_NAME);
	if(IS_ERR(device_name)) {
		class_destroy(class_name);
		unregister_chrdev_region(dev, 1);
		printk(KERN_INFO "Failed to Create Device");
		return PTR_ERR(device_name);
	}
	printk(KERN_INFO "Device Created Successfully");

	cdev_init(&char_dev, &Fops);
	if(cdev_add(&char_dev, dev, 1) == -1) {
		device_destroy(class_name, dev);
		class_destroy(class_name);
		unregister_chrdev_region(dev, 1);
		return -1;
	}
	return 0;
}

static void __exit piio_driver_exit(void) {
	cdev_del(&char_dev);
	device_destroy(class_name, dev);
	class_destroy(class_name);
	unregister_chrdev_region(dev, 1);
	printk(KERN_INFO "PiIO LKM Removed Successfully\n");
}

module_init(piio_driver_init);
module_exit(piio_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PAUL CRUZ");
MODULE_DESCRIPTION("Control for RPi GPIO Pin");
MODULE_VERSION("1.0");
