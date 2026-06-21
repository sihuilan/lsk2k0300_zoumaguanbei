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
	{ 0x8487a2b6, "flush_work" },
	{ 0x2d3385d3, "system_wq" },
	{ 0xac0d64e5, "bus_register" },
	{ 0x402b8281, "__request_module" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x8f447bb0, "driver_register" },
	{ 0x8644804e, "seq_printf" },
	{ 0x88bfa7e, "cancel_work_sync" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x156f81f0, "snd_device_new" },
	{ 0x3b4c918a, "device_del" },
	{ 0x7c32d0f0, "printk" },
	{ 0xd438ff09, "driver_unregister" },
	{ 0xb13ec247, "device_add" },
	{ 0xcd401818, "bus_unregister" },
	{ 0x8ed557d2, "bus_for_each_dev" },
	{ 0xa61db944, "put_device" },
	{ 0x37a0cba, "kfree" },
	{ 0x86ed32c4, "snd_seq_root" },
	{ 0x9de632a0, "snd_info_free_entry" },
	{ 0x46041cc4, "device_initialize" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0x6fed1cec, "dev_set_name" },
	{ 0xf07d8caf, "snd_info_create_module_entry" },
	{ 0x7222467c, "snd_info_register" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

