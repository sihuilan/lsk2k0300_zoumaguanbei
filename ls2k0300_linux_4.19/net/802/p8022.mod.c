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
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x2698919f, "llc_sap_open" },
	{ 0x37a0cba, "kfree" },
	{ 0xe6df13fb, "llc_sap_close" },
	{ 0x99b22ec, "llc_build_and_send_ui_pkt" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=llc";

