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
	{ 0x6f8c9a67, "__platform_driver_register" },
	{ 0xf9a482f9, "msleep" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x12a38747, "usleep_range" },
	{ 0x37a0cba, "kfree" },
	{ 0x932cd3b0, "gpiod_set_array_value_cansleep" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x2ec13e1f, "mmc_pwrseq_register" },
	{ 0xf3f3f58e, "device_property_read_u32_array" },
	{ 0xefb39c1a, "devm_gpiod_get_array" },
	{ 0x33f7c8d4, "devm_kmalloc" },
	{ 0x73558a52, "mmc_pwrseq_unregister" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cmmc-pwrseq-simple");
MODULE_ALIAS("of:N*T*Cmmc-pwrseq-simpleC*");
