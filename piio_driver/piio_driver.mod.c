#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0xfece093d, "module_layout" },
	{ 0x122b39f2, "cdev_del" },
	{ 0xe8be6d2f, "device_destroy" },
	{ 0x4eb5f84b, "cdev_add" },
	{ 0xfd978014, "cdev_init" },
	{ 0xd003e9c3, "class_destroy" },
	{ 0x82a137a5, "device_create" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x9b09fde, "__class_create" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x5f754e5a, "memset" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0xd1915fde, "gpiod_set_raw_value" },
	{ 0x2724c21b, "gpiod_direction_output_raw" },
	{ 0x455de5e3, "gpio_to_desc" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0xeae2c290, "try_module_get" },
	{ 0xa7e36f1a, "module_put" },
	{ 0xc5850110, "printk" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "7FFC3D4D1EF6DC785DD9BEB");
