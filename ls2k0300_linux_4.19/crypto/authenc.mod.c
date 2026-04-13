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
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x7936b376, "scatterwalk_ffwd" },
	{ 0xd6b570a3, "crypto_init_ahash_spawn" },
	{ 0x7014e612, "crypto_mod_put" },
	{ 0xdef59361, "ahash_attr_alg" },
	{ 0x9a11a0fc, "crypto_attr_alg_name" },
	{ 0x3baac85f, "crypto_grab_skcipher" },
	{ 0xef25b6de, "crypto_unregister_template" },
	{ 0x8d7a2a4d, "crypto_get_default_null_skcipher" },
	{ 0xd8ff470f, "scatterwalk_map_and_copy" },
	{ 0x21cd536a, "crypto_put_default_null_skcipher" },
	{ 0xdc3c41a8, "__crypto_memneq" },
	{ 0x264af841, "crypto_ahash_digest" },
	{ 0xd9a0665e, "crypto_register_template" },
	{ 0xea47d6b7, "crypto_drop_spawn" },
	{ 0xfdbd7a17, "crypto_get_attr_type" },
	{ 0x6c240284, "crypto_destroy_tfm" },
	{ 0xc15a44c6, "memzero_explicit" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x20c3c1aa, "crypto_spawn_tfm2" },
	{ 0x37a0cba, "kfree" },
	{ 0xa7cc3d67, "crypto_ahash_setkey" },
	{ 0x28318305, "snprintf" },
	{ 0xecc304f8, "aead_register_instance" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

