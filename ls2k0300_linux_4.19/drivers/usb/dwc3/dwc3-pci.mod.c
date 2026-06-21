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
	{ 0x54f985aa, "pci_unregister_driver" },
	{ 0x5d90053, "__pci_register_driver" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0x7d59dd46, "pm_wq" },
	{ 0x37a0cba, "kfree" },
	{ 0xae5a04bb, "acpi_evaluate_dsm" },
	{ 0x12a38747, "usleep_range" },
	{ 0x5f203ee2, "gpiod_put" },
	{ 0x121a60de, "gpiod_set_value_cansleep" },
	{ 0x24ae5e0d, "gpiod_get_optional" },
	{ 0x1d4e2af4, "gpiod_add_lookup_table" },
	{ 0x461d5c82, "devm_acpi_dev_add_driver_gpios" },
	{ 0xd792eee8, "pcim_iounmap" },
	{ 0xf9a482f9, "msleep" },
	{ 0xf3936912, "pcim_iomap" },
	{ 0xb89b6e6b, "guid_parse" },
	{ 0x506730d9, "platform_device_put" },
	{ 0x33dce5f6, "_dev_err" },
	{ 0xa099a3f7, "__pm_runtime_idle" },
	{ 0x675bfdc, "platform_device_add" },
	{ 0x54b35d8f, "platform_device_add_properties" },
	{ 0x7b063069, "set_primary_fwnode" },
	{ 0x4aaaddd3, "is_acpi_device_node" },
	{ 0x3ac3421d, "platform_device_add_resources" },
	{ 0xfb578fc5, "memset" },
	{ 0x7c188cbb, "platform_device_alloc" },
	{ 0x33f7c8d4, "devm_kmalloc" },
	{ 0x794aef33, "pci_set_master" },
	{ 0x364adcc9, "pcim_enable_device" },
	{ 0xee8f3c66, "__pm_runtime_suspend" },
	{ 0x526c3a6c, "jiffies" },
	{ 0x1c1454f9, "gpiod_remove_lookup_table" },
	{ 0x3f1fb11a, "platform_device_unregister" },
	{ 0x53adbfb8, "__pm_runtime_resume" },
	{ 0x60dedd09, "device_init_wakeup" },
	{ 0x88bfa7e, "cancel_work_sync" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v00008086d000022B7sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00000F37sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000119Esv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000002EEsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000006EEsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00009D30sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000A130sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00000AAAsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00001AAAsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00005AAAsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000A2B0sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000031AAsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d00009DEEsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000A36Esv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d0000A3B0sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008086d000034EEsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001022d00007912sv*sd*bc*sc*i*");
