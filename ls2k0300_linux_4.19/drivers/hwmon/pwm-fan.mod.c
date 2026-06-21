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
	{ 0x9f304cf1, "thermal_cdev_update" },
	{ 0x267ca317, "thermal_of_cooling_device_register" },
	{ 0xbaf460f9, "of_property_read_variable_u32_array" },
	{ 0x950c9c41, "of_property_count_elems_of_size" },
	{ 0xc4142202, "of_find_property" },
	{ 0xe222b39, "devm_hwmon_device_register_with_groups" },
	{ 0x93c2fff0, "devm_of_pwm_get" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0x33f7c8d4, "devm_kmalloc" },
	{ 0x91715312, "sprintf" },
	{ 0x60ea2d6, "kstrtoull" },
	{ 0x33dce5f6, "_dev_err" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0x10c2e328, "thermal_cooling_device_unregister" },
	{ 0xc4668deb, "pwm_apply_state" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Cpwm-fan");
MODULE_ALIAS("of:N*T*Cpwm-fanC*");
