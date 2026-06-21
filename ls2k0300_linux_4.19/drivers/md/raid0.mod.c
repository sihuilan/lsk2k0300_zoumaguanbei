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
	{ 0x2af59d26, "param_ops_int" },
	{ 0x7e8175c, "unregister_md_personality" },
	{ 0x503f81a, "register_md_personality" },
	{ 0x3dd1af68, "blk_queue_flag_clear" },
	{ 0xef5bb302, "md_integrity_register" },
	{ 0xb996bd76, "md_set_array_sectors" },
	{ 0xda3de13f, "blk_queue_flag_set" },
	{ 0xe06dfe09, "disk_stack_limits" },
	{ 0x9f9dfaf2, "blk_queue_io_opt" },
	{ 0x6652186e, "blk_queue_io_min" },
	{ 0x83b42f4a, "blk_queue_max_discard_sectors" },
	{ 0x4d385893, "blk_queue_max_write_zeroes_sectors" },
	{ 0x7f34fa61, "blk_queue_max_write_same_sectors" },
	{ 0xe64ddd3c, "blk_queue_max_hw_sectors" },
	{ 0x122bb871, "md_check_no_bitmap" },
	{ 0x28318305, "snprintf" },
	{ 0x6c7e0633, "bdevname" },
	{ 0x3d9f21fe, "__cpu_online_mask" },
	{ 0x7ef7b4bb, "__blkdev_issue_discard" },
	{ 0xbfd3a0da, "md_flush_request" },
	{ 0x5b081498, "bio_chain" },
	{ 0xc7d2f5a, "bio_split" },
	{ 0x98a8dc65, "generic_make_request" },
	{ 0x80f7d128, "__tracepoint_block_bio_remap" },
	{ 0x1c9bef99, "bio_endio" },
	{ 0x8644804e, "seq_printf" },
	{ 0xad27f361, "__warn_printk" },
	{ 0xa202a8e5, "kmalloc_order_trace" },
	{ 0x7c32d0f0, "printk" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=md-mod";

