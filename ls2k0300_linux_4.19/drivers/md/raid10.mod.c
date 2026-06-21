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
	{ 0xde65062, "md_finish_reshape" },
	{ 0x241ffa74, "md_bitmap_end_sync" },
	{ 0x16a261c0, "bio_alloc_mddev" },
	{ 0xf2d0b6fa, "sysfs_notify" },
	{ 0x151f4898, "schedule_timeout_uninterruptible" },
	{ 0x34888a2c, "md_bitmap_close_sync" },
	{ 0x4c6ed374, "md_bitmap_start_sync" },
	{ 0xecd04ce9, "md_bitmap_cond_end_sync" },
	{ 0x1c3e02e4, "memcmp" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0x375eb73e, "bio_copy_data" },
	{ 0x61e7f3a2, "bio_add_page" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0x7e44f4b5, "rdev_clear_badblocks" },
	{ 0x9769f67c, "bio_trim" },
	{ 0x637e7173, "submit_bio_wait" },
	{ 0xe6601de1, "md_check_recovery" },
	{ 0xa9320d27, "ktime_get_seconds" },
	{ 0xbfd3a0da, "md_flush_request" },
	{ 0xfb578fc5, "memset" },
	{ 0x8b4a571a, "md_write_start" },
	{ 0xe841c7b1, "md_wait_for_blocked_rdev" },
	{ 0x74d5f85a, "md_bitmap_startwrite" },
	{ 0xd5fd90f1, "prepare_to_wait" },
	{ 0xad73041f, "autoremove_wake_function" },
	{ 0xe914e41e, "strcpy" },
	{ 0x5b081498, "bio_chain" },
	{ 0xc7d2f5a, "bio_split" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x1d24c881, "___ratelimit" },
	{ 0x11cb5bde, "md_new_event" },
	{ 0x526c3a6c, "jiffies" },
	{ 0x7d9b7eeb, "sysfs_create_link" },
	{ 0x91715312, "sprintf" },
	{ 0xd82184ff, "md_integrity_add_rdev" },
	{ 0xc2cdbf1, "synchronize_sched" },
	{ 0xa7907a4b, "kernfs_notify" },
	{ 0x3d9f21fe, "__cpu_online_mask" },
	{ 0xd27b25dd, "blk_check_plugged" },
	{ 0x80f7d128, "__tracepoint_block_bio_remap" },
	{ 0x917c91ef, "bio_clone_fast" },
	{ 0x6ad5d038, "md_do_sync" },
	{ 0xef5bb302, "md_integrity_register" },
	{ 0x3dd1af68, "blk_queue_flag_clear" },
	{ 0xda3de13f, "blk_queue_flag_set" },
	{ 0xf9e65279, "md_unregister_thread" },
	{ 0xe06dfe09, "disk_stack_limits" },
	{ 0x9f9dfaf2, "blk_queue_io_opt" },
	{ 0x6652186e, "blk_queue_io_min" },
	{ 0x4d385893, "blk_queue_max_write_zeroes_sectors" },
	{ 0x7f34fa61, "blk_queue_max_write_same_sectors" },
	{ 0x83b42f4a, "blk_queue_max_discard_sectors" },
	{ 0x2bde7038, "mddev_init_writes_pending" },
	{ 0x6c7e0633, "bdevname" },
	{ 0x8644804e, "seq_printf" },
	{ 0x38ea4d92, "bio_reset" },
	{ 0x86c45796, "mempool_alloc" },
	{ 0x4fcf188b, "memcpy" },
	{ 0xc9c79ae5, "bio_alloc_bioset" },
	{ 0xb996bd76, "md_set_array_sectors" },
	{ 0x32258540, "md_bitmap_resize" },
	{ 0x7c32d0f0, "printk" },
	{ 0xd7508347, "md_register_thread" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x4fe481e0, "bioset_init" },
	{ 0x4d754f2b, "mempool_init" },
	{ 0xe38d270d, "__alloc_pages_nodemask" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x943f06fa, "bio_put" },
	{ 0x235a7fff, "__put_page" },
	{ 0xc10e16c7, "bioset_exit" },
	{ 0x36c2dd1d, "mempool_exit" },
	{ 0xad27f361, "__warn_printk" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x1000e51, "schedule" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x34d4bdea, "md_error" },
	{ 0xc87d6aba, "rdev_set_badblocks" },
	{ 0x6d937600, "sync_page_io" },
	{ 0x6160b2b3, "badblocks_check" },
	{ 0x4d044163, "md_cluster_ops" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x85570612, "md_done_sync" },
	{ 0x3735d260, "md_wakeup_thread" },
	{ 0xbd9074b1, "blk_finish_plug" },
	{ 0x1c9bef99, "bio_endio" },
	{ 0x98a8dc65, "generic_make_request" },
	{ 0x33fc23cc, "md_bitmap_unplug" },
	{ 0x7d705738, "blk_start_plug" },
	{ 0x74b55fe, "mempool_free" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0xea3a00d8, "md_write_end" },
	{ 0x8c41608e, "md_bitmap_endwrite" },
	{ 0x37a0cba, "kfree" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=md-mod";

