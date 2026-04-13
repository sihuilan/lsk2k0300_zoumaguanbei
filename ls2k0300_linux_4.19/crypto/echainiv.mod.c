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
	{ 0xef25b6de, "crypto_unregister_template" },
	{ 0xd9a0665e, "crypto_register_template" },
	{ 0xfb578fc5, "memset" },
	{ 0x4fcf188b, "memcpy" },
	{ 0xd8ff470f, "scatterwalk_map_and_copy" },
	{ 0xecc304f8, "aead_register_instance" },
	{ 0xc8a84757, "aead_exit_geniv" },
	{ 0xb57de43c, "aead_init_geniv" },
	{ 0x94715d73, "aead_geniv_alloc" },
	{ 0x3db2fdb2, "aead_geniv_free" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

