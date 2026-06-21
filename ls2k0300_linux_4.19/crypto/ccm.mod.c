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
	{ 0x5c8fa1cc, "shash_register_instance" },
	{ 0x27b88ade, "shash_free_instance" },
	{ 0x4fc9d69b, "crypto_init_spawn" },
	{ 0xe86621a3, "crypto_alloc_instance2" },
	{ 0xc923401, "crypto_attr_alg2" },
	{ 0x243f0b4b, "crypto_check_attr_type" },
	{ 0xa5338093, "crypto_spawn_tfm" },
	{ 0xa3f12f69, "__crypto_xor" },
	{ 0xa7cc3d67, "crypto_ahash_setkey" },
	{ 0xdc3c41a8, "__crypto_memneq" },
	{ 0xc4ed5445, "sg_next" },
	{ 0x3e0634d, "crypto_ahash_finup" },
	{ 0xfb578fc5, "memset" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x3baac85f, "crypto_grab_skcipher" },
	{ 0xd6b570a3, "crypto_init_ahash_spawn" },
	{ 0x7014e612, "crypto_mod_put" },
	{ 0x5a921311, "strncmp" },
	{ 0x133c6113, "crypto_find_alg" },
	{ 0xd3370baa, "crypto_ahash_type" },
	{ 0xecc304f8, "aead_register_instance" },
	{ 0x28318305, "snprintf" },
	{ 0x37a0cba, "kfree" },
	{ 0xea47d6b7, "crypto_drop_spawn" },
	{ 0x6b39fd98, "crypto_grab_aead" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x9a11a0fc, "crypto_attr_alg_name" },
	{ 0xfdbd7a17, "crypto_get_attr_type" },
	{ 0x20c3c1aa, "crypto_spawn_tfm2" },
	{ 0x6c240284, "crypto_destroy_tfm" },
	{ 0x9106c583, "crypto_aead_setkey" },
	{ 0x42d4810e, "crypto_aead_setauthsize" },
	{ 0x7936b376, "scatterwalk_ffwd" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0x50b03c47, "sg_init_table" },
	{ 0xd8ff470f, "scatterwalk_map_and_copy" },
	{ 0x4fcf188b, "memcpy" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

