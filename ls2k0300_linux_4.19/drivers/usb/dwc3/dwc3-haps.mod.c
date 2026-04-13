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
	{ 0x33dce5f6, "_dev_err" },
	{ 0x675bfdc, "platform_device_add" },
	{ 0x506730d9, "platform_device_put" },
	{ 0x54b35d8f, "platform_device_add_properties" },
	{ 0x3ac3421d, "platform_device_add_resources" },
	{ 0xfb578fc5, "memset" },
	{ 0x7c188cbb, "platform_device_alloc" },
	{ 0x33f7c8d4, "devm_kmalloc" },
	{ 0x794aef33, "pci_set_master" },
	{ 0x364adcc9, "pcim_enable_device" },
	{ 0x3f1fb11a, "platform_device_unregister" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v000016C3d0000ABCDsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000016C3d0000ABCEsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000016C3d0000ABCFsv*sd*bc*sc*i*");
