#ifndef PIIO_DRIVER_H
#define PIIO_DRIVER_H

#include <linux/ioctl.h>

typedef struct lkm_data {
	unsigned char data[256];
} lkm_data;

typedef struct gpio_pin {
	char desc[16];
	unsigned int green_pin;
	unsigned int red_pin;
	int green_value;
	int red_value;
} gpio_pin;

#define IOCTL_PIIO_GPIO_GREEN_LED_WRITE 0x67
#define IOCTL_PIIO_GPIO_RED_LED_WRITE 0x68

#define DEVICE_NAME "piio_driver_dev"
#define CLASS_NAME "piio_driver_class"

#endif
