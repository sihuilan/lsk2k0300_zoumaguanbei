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
	{ 0x2a2bd98e, "crypto_alloc_skcipher" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0x50b03c47, "sg_init_table" },
	{ 0x38d3dce5, "g_make_token_header" },
	{ 0x4d41fc62, "crypto_alloc_shash" },
	{ 0x98cf60b3, "strlen" },
	{ 0xcf74d771, "write_bytes_to_xdr_buf" },
	{ 0x448eac3e, "kmemdup" },
	{ 0xf54bd49b, "lcm" },
	{ 0x5758fe5c, "gss_mech_register" },
	{ 0x953e1b9e, "ktime_get_real_seconds" },
	{ 0xdc3c41a8, "__crypto_memneq" },
	{ 0x1e76df6f, "crypto_ahash_final" },
	{ 0x264af841, "crypto_ahash_digest" },
	{ 0xd7673035, "g_verify_token_header" },
	{ 0xade8de46, "xdr_buf_subsegment" },
	{ 0xfb578fc5, "memset" },
	{ 0x7c32d0f0, "printk" },
	{ 0x1c3e02e4, "memcmp" },
	{ 0x4c1cfc6f, "crypto_shash_digest" },
	{ 0xfda9581f, "prandom_u32" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0xeed6ab63, "crypto_shash_setkey" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x6c240284, "crypto_destroy_tfm" },
	{ 0xb7a3d32, "gss_mech_unregister" },
	{ 0xc15a44c6, "memzero_explicit" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x79ff628e, "sg_init_one" },
	{ 0x37a0cba, "kfree" },
	{ 0x4fcf188b, "memcpy" },
	{ 0xa7cc3d67, "crypto_ahash_setkey" },
	{ 0x7a4497db, "kzfree" },
	{ 0xd13f9038, "read_bytes_from_xdr_buf" },
	{ 0x5fd571c8, "xdr_process_buf" },
	{ 0xbe66560, "memmove" },
	{ 0x482ac5a4, "g_token_size" },
	{ 0xb9ef4ada, "xdr_buf_trim" },
	{ 0xdcc58999, "crypto_alloc_ahash" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

