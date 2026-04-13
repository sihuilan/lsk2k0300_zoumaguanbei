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
	{ 0xb27ad9e2, "crypto_unregister_shash" },
	{ 0xf46b6258, "crypto_register_shash" },
	{ 0xfb578fc5, "memset" },
	{ 0x5a4d313e, "gf128mul_4k_lle" },
	{ 0xa3f12f69, "__crypto_xor" },
	{ 0xc15a44c6, "memzero_explicit" },
	{ 0x83581089, "gf128mul_init_4k_lle" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x7a4497db, "kzfree" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=gf128mul";

