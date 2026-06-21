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
	{ 0x2af59d26, "param_ops_int" },
	{ 0x433ff50d, "param_array_ops" },
	{ 0x81e1f87a, "noop_llseek" },
	{ 0x3d85d98, "idr_destroy" },
	{ 0xd438ff09, "driver_unregister" },
	{ 0xe07aac8e, "class_destroy" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xa912d2b2, "scsi_register_driver" },
	{ 0x5a137d44, "__register_chrdev" },
	{ 0x4c989a74, "__class_create" },
	{ 0x7c32d0f0, "printk" },
	{ 0xe117ec8, "scsi_device_lookup" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0x368ad888, "device_create" },
	{ 0x91715312, "sprintf" },
	{ 0x7d79ea60, "idr_alloc" },
	{ 0x84ffea8b, "idr_preload" },
	{ 0x9166fada, "strncpy" },
	{ 0xabb3bcd2, "scsi_ioctl" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xe526f136, "__copy_user" },
	{ 0xd621cfe, "scsi_ioctl_block_when_processing_errors" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x9cb5b641, "sdev_prefix_printk" },
	{ 0xfb578fc5, "memset" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0xf4764e8f, "scsi_print_sense_hdr" },
	{ 0xadf7dd37, "__scsi_execute" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0xa706c25b, "scsi_device_get" },
	{ 0x9ad03057, "idr_find" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0xa9633f37, "scsi_device_put" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x37a0cba, "kfree" },
	{ 0x72d2e962, "device_destroy" },
	{ 0xfe6e59fa, "idr_remove" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

