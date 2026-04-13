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
	{ 0xeb38711b, "blkdev_issue_discard" },
	{ 0x9c38cd9f, "kmem_cache_destroy" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x7129e5f8, "hex_asc" },
	{ 0xbd829c21, "sb_min_blocksize" },
	{ 0xbd6c2603, "mark_buffer_dirty_inode" },
	{ 0xdb38c297, "up_read" },
	{ 0xe7f1326a, "unload_nls" },
	{ 0x78470c5, "generic_file_llseek" },
	{ 0x61057820, "__mark_inode_dirty" },
	{ 0x60a13e90, "rcu_barrier" },
	{ 0x250db867, "__generic_file_fsync" },
	{ 0xec7bc422, "from_kuid_munged" },
	{ 0x815b5dd4, "match_octal" },
	{ 0x64999478, "congestion_wait" },
	{ 0xa375d42f, "generic_fh_to_parent" },
	{ 0x75063ef6, "seq_puts" },
	{ 0x8bec0158, "blkdev_issue_flush" },
	{ 0xacf4d843, "match_strdup" },
	{ 0xfff5afc, "time64_to_tm" },
	{ 0xfe37d3d1, "__lock_buffer" },
	{ 0xabf32f29, "utf16s_to_utf8s" },
	{ 0x8644804e, "seq_printf" },
	{ 0x44e9a829, "match_token" },
	{ 0x6634eaae, "init_user_ns" },
	{ 0x3096be16, "names_cachep" },
	{ 0x8ff3d267, "filemap_fdatawait_range" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x85df9b6c, "strsep" },
	{ 0x8e2d3ae9, "generic_read_dir" },
	{ 0x75132049, "__getblk_gfp" },
	{ 0x25851673, "igrab" },
	{ 0x25fbbd1a, "unlock_buffer" },
	{ 0x91715312, "sprintf" },
	{ 0x953e1b9e, "ktime_get_real_seconds" },
	{ 0xe44d0c18, "truncate_setsize" },
	{ 0xadaf718e, "down_read" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x82f169fa, "from_kgid_munged" },
	{ 0x9d0e7cf1, "make_kgid" },
	{ 0xe526f136, "__copy_user" },
	{ 0x218cf29, "__insert_inode_hash" },
	{ 0xfd5142d1, "mpage_readpages" },
	{ 0xfb578fc5, "memset" },
	{ 0x67db6d90, "mpage_readpage" },
	{ 0x59ac3e11, "__bread_gfp" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0x7c32d0f0, "printk" },
	{ 0xa9e3bd89, "d_obtain_alias" },
	{ 0x1c3e02e4, "memcmp" },
	{ 0xf120315e, "write_inode_now" },
	{ 0x7c1372e8, "panic" },
	{ 0xabef749f, "mpage_writepages" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x5a921311, "strncmp" },
	{ 0xe5a21497, "kmem_cache_free" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0x8bba9570, "set_nlink" },
	{ 0xbe0adb70, "__wait_on_buffer" },
	{ 0x95d78776, "setattr_copy" },
	{ 0xf410df9f, "sync_dirty_buffer" },
	{ 0x9fd92c89, "truncate_pagecache" },
	{ 0x4e3567f7, "match_int" },
	{ 0xe9ed4df1, "generic_file_read_iter" },
	{ 0xe6bbbe4, "up_write" },
	{ 0x6a768497, "down_write" },
	{ 0x6df8e7c3, "__brelse" },
	{ 0xfe5d4bb2, "sys_tz" },
	{ 0x528e2783, "__fsnotify_parent" },
	{ 0xc136b8a3, "inode_init_once" },
	{ 0x5b9ea23a, "mnt_drop_write_file" },
	{ 0xc6cbbc89, "capable" },
	{ 0x19845ffc, "invalidate_inode_buffers" },
	{ 0x57a0c51d, "kmem_cache_alloc" },
	{ 0x44f7edab, "ilookup" },
	{ 0xd82f49e8, "sync_mapping_buffers" },
	{ 0xe2c871af, "generic_file_mmap" },
	{ 0x652c08b4, "block_write_full_page" },
	{ 0xb11ac64, "truncate_inode_pages_final" },
	{ 0xbc601b3, "make_kuid" },
	{ 0x22c502aa, "generic_cont_expand_simple" },
	{ 0xf7b8344, "load_nls" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x1d24c881, "___ratelimit" },
	{ 0x58f1150b, "generic_write_end" },
	{ 0xd98dfdb2, "__breadahead" },
	{ 0x5a829d01, "mnt_want_write_file" },
	{ 0x8b8059bd, "in_group_p" },
	{ 0x5e95b1cd, "current_umask" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x110e6c3f, "sync_inode_metadata" },
	{ 0xfece5f82, "kmem_cache_create" },
	{ 0xf8553fa9, "generic_file_write_iter" },
	{ 0x9485496c, "iput" },
	{ 0x1561e70f, "cont_write_begin" },
	{ 0x37a0cba, "kfree" },
	{ 0xaf4bd5db, "iunique" },
	{ 0x32c1e1a3, "inode_dio_wait" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x643e0ce5, "call_rcu_sched" },
	{ 0xf75031f3, "current_time" },
	{ 0x4be1a557, "fsnotify" },
	{ 0x752bb08e, "sync_filesystem" },
	{ 0x9ceb7875, "write_dirty_buffer" },
	{ 0x46d5a8e, "block_truncate_page" },
	{ 0xb65681c, "sb_set_blocksize" },
	{ 0xa3d6ac1e, "__bforget" },
	{ 0xb76e5e5c, "d_make_root" },
	{ 0xfd83335, "__blockdev_direct_IO" },
	{ 0x4f5c62b2, "inode_needs_sync" },
	{ 0x89db376a, "filemap_fdatawrite_range" },
	{ 0x38f3bb20, "__find_get_block" },
	{ 0xda7da9a2, "security_inode_setattr" },
	{ 0xc0eadf41, "mark_buffer_dirty" },
	{ 0xce45ff20, "new_inode" },
	{ 0x11aa9416, "generic_file_splice_read" },
	{ 0x313ac16, "generic_fh_to_dentry" },
	{ 0xd0b8a33e, "clear_inode" },
	{ 0x4edd04ae, "__init_rwsem" },
	{ 0x380a3035, "filemap_flush" },
	{ 0x662c0ae6, "generic_block_bmap" },
	{ 0x812f2f4, "setattr_prepare" },
	{ 0x9024b904, "generic_fillattr" },
	{ 0x6ee61e17, "filemap_fdatawrite" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

