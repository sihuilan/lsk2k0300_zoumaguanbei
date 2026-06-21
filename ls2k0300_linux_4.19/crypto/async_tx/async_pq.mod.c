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
	{ 0xa8717049, "loongson_dma_ops" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0x6680e7aa, "raid6_empty_zero_page" },
	{ 0xe38d270d, "__alloc_pages_nodemask" },
	{ 0x80da184d, "dmaengine_unmap_put" },
	{ 0x2b30f429, "raid6_call" },
	{ 0xce9ecc5, "async_tx_submit" },
	{ 0x7c32d0f0, "printk" },
	{ 0x1c3e02e4, "memcmp" },
	{ 0x14833ac7, "dmaengine_get_unmap_data" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0x52d1162f, "__async_tx_find_channel" },
	{ 0x84262d84, "__free_pages" },
	{ 0x8393378, "async_xor" },
	{ 0xc432df6f, "async_tx_quiesce" },
	{ 0x17f54263, "raid6_gfexp" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=raid6_pq,async_tx,async_xor";

