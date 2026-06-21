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
	{ 0xa8717049, "loongson_dma_ops" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0xad27f361, "__warn_printk" },
	{ 0x80da184d, "dmaengine_unmap_put" },
	{ 0xfb578fc5, "memset" },
	{ 0xce9ecc5, "async_tx_submit" },
	{ 0x5b6c00e6, "xor_blocks" },
	{ 0x14833ac7, "dmaengine_get_unmap_data" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0x8df92f66, "memchr_inv" },
	{ 0x52d1162f, "__async_tx_find_channel" },
	{ 0xc432df6f, "async_tx_quiesce" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=async_tx,xor";

