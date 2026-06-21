#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

MODULE_INFO(intree, "Y");

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xe3ae0b4d, "module_layout" },
	{ 0x81e1f87a, "noop_llseek" },
	{ 0x2af59d26, "param_ops_int" },
	{ 0xd807ba3e, "blkdev_fsync" },
	{ 0x3e2f4bdf, "blkdev_write_iter" },
	{ 0x82dee513, "blkdev_read_iter" },
	{ 0xb5b30e0f, "default_llseek" },
	{ 0xe07aac8e, "class_destroy" },
	{ 0x999e8297, "vfree" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xd239f36b, "cdev_del" },
	{ 0x4c989a74, "__class_create" },
	{ 0x772a8264, "cdev_add" },
	{ 0xc382d164, "cdev_init" },
	{ 0x3fd78f3b, "register_chrdev_region" },
	{ 0x40a9b349, "vzalloc" },
	{ 0x7c32d0f0, "printk" },
	{ 0x5d02b57f, "blkdev_ioctl" },
	{ 0xfb578fc5, "memset" },
	{ 0x368ad888, "device_create" },
	{ 0x2d35756d, "__module_get" },
	{ 0xb346fabb, "bdget" },
	{ 0x72d2e962, "device_destroy" },
	{ 0x4322b27b, "module_put" },
	{ 0x4e6ddf9c, "bdput" },
	{ 0xc6cbbc89, "capable" },
	{ 0xe526f136, "__copy_user" },
	{ 0xdc45fc14, "set_blocksize" },
	{ 0x49d431cc, "blkdev_get" },
	{ 0x863e8f90, "bdgrab" },
	{ 0x93c15c00, "blkdev_put" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0x44b5ee9a, "kasprintf" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

