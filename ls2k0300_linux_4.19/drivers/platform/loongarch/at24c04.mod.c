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
	{ 0x4014357a, "platform_driver_unregister" },
	{ 0x3f1fb11a, "platform_device_unregister" },
	{ 0xf4ddf8aa, "platform_device_register_full" },
	{ 0x6f8c9a67, "__platform_driver_register" },
	{ 0x7c32d0f0, "printk" },
	{ 0x9cda8213, "i2c_new_device" },
	{ 0x9166fada, "strncpy" },
	{ 0x5a921311, "strncmp" },
	{ 0xcbd40aea, "i2c_get_adapter" },
	{ 0xfb578fc5, "memset" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

