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
	{ 0x4c2d96f1, "_dev_notice" },
	{ 0x2ec13e1f, "mmc_pwrseq_register" },
	{ 0xd9d8fd16, "register_restart_handler" },
	{ 0xaea275b2, "gpiod_cansleep" },
	{ 0xc974404e, "devm_gpiod_get" },
	{ 0x33f7c8d4, "devm_kmalloc" },
	{ 0x53a703be, "gpiod_set_value" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0x121a60de, "gpiod_set_value_cansleep" },
	{ 0x73558a52, "mmc_pwrseq_unregister" },
	{ 0x40d59096, "unregister_restart_handler" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cmmc-pwrseq-emmc");
MODULE_ALIAS("of:N*T*Cmmc-pwrseq-emmcC*");
