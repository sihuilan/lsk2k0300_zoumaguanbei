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
	{ 0xb898a5c9, "param_ops_bool" },
	{ 0xa71966d3, "dm_unregister_target" },
	{ 0xde780efb, "dm_register_target" },
	{ 0x2395ee04, "raid5_set_cache_size" },
	{ 0x747c791e, "r5c_journal_mode_set" },
	{ 0xb1425b32, "dm_table_add_target_callbacks" },
	{ 0xa10ca5c3, "md_start" },
	{ 0xed9ccd9b, "md_run" },
	{ 0xe38d270d, "__alloc_pages_nodemask" },
	{ 0x6d4a7905, "dm_get_device" },
	{ 0x6d0f1f89, "dm_table_get_mode" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x5c63396f, "dm_set_target_max_io_len" },
	{ 0xa202a8e5, "kmalloc_order_trace" },
	{ 0x373db350, "kstrtoint" },
	{ 0xa91fbfc6, "md_rdev_init" },
	{ 0x555a5a9e, "mddev_init" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x11e0ec41, "dm_read_arg" },
	{ 0x7deff673, "dm_consume_args" },
	{ 0xc7d094b5, "dm_read_arg_group" },
	{ 0x5eb24829, "dm_shift_arg" },
	{ 0x4ca9669f, "scnprintf" },
	{ 0x3fe2ccbe, "memweight" },
	{ 0xad84bef8, "dm_table_event" },
	{ 0x243079fd, "revalidate_disk" },
	{ 0x1325830e, "dm_disk" },
	{ 0x376b7589, "dm_table_get_md" },
	{ 0x4d7ba1a9, "mddev_resume" },
	{ 0x34d4bdea, "md_error" },
	{ 0x6d937600, "sync_page_io" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0xfb578fc5, "memset" },
	{ 0x3735d260, "md_wakeup_thread" },
	{ 0xa1dc4c1b, "md_reap_sync_thread" },
	{ 0xaafdc258, "strcasecmp" },
	{ 0x49d42c8, "mddev_congested" },
	{ 0xb5897c2a, "md_stop" },
	{ 0x37a0cba, "kfree" },
	{ 0x993b15e1, "dm_put_device" },
	{ 0x35368ca, "md_rdev_clear" },
	{ 0xe701744f, "md_handle_request" },
	{ 0x59560f15, "md_stop_writes" },
	{ 0xe0b5becf, "mddev_suspend" },
	{ 0x39f09c10, "md_bitmap_load" },
	{ 0x32258540, "md_bitmap_resize" },
	{ 0x9a9cd64c, "mddev_unlock" },
	{ 0x5600efbd, "md_update_sb" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0x7c32d0f0, "printk" },
	{ 0x74c134b9, "__sw_hweight32" },
	{ 0xdc9596bf, "blk_limits_io_opt" },
	{ 0x829b05dc, "blk_limits_io_min" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=raid456,md-mod";

