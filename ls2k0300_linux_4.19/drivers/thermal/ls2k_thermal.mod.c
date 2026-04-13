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
	{ 0x33dce5f6, "_dev_err" },
	{ 0x6cd99c06, "thermal_add_hwmon_sysfs" },
	{ 0xe5792ba0, "devm_request_threaded_irq" },
	{ 0xe686af97, "devm_thermal_zone_of_sensor_register" },
	{ 0xbaf460f9, "of_property_read_variable_u32_array" },
	{ 0x64c9b254, "platform_get_irq" },
	{ 0x60cb2ec6, "devm_ioremap" },
	{ 0xae094dae, "platform_get_resource" },
	{ 0x33f7c8d4, "devm_kmalloc" },
	{ 0x27bbf221, "disable_irq_nosync" },
	{ 0xfcec0987, "enable_irq" },
	{ 0xfc42e7d5, "thermal_zone_device_update" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cloongson,ls2k-tsensor");
MODULE_ALIAS("of:N*T*Cloongson,ls2k-tsensorC*");
