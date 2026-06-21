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
	{ 0x34b5cb3d, "fat_detach" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x39c7e78d, "drop_nlink" },
	{ 0x97adb487, "utf8s_to_utf16s" },
	{ 0xbd6c2603, "mark_buffer_dirty_inode" },
	{ 0x61057820, "__mark_inode_dirty" },
	{ 0x13d8bd6d, "dput" },
	{ 0xfb819dc4, "inc_nlink" },
	{ 0xa335309a, "d_find_alias" },
	{ 0x3096be16, "names_cachep" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x1bbfa54e, "mount_bdev" },
	{ 0x7b7b024a, "fat_sync_inode" },
	{ 0x3db38363, "fat_add_entries" },
	{ 0x526c3a6c, "jiffies" },
	{ 0x6cb8aa67, "fat_remove_entries" },
	{ 0x1d00fff7, "fat_alloc_new_dir" },
	{ 0xfb578fc5, "memset" },
	{ 0xfb3afe72, "fat_fill_super" },
	{ 0x6aa60089, "fat_build_inode" },
	{ 0x11089ac7, "_ctype" },
	{ 0x65d66261, "fat_attach" },
	{ 0x2ed06d9f, "d_move" },
	{ 0x5a921311, "strncmp" },
	{ 0xe5a21497, "kmem_cache_free" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0x8bba9570, "set_nlink" },
	{ 0xf410df9f, "sync_dirty_buffer" },
	{ 0xdd82b12, "fat_getattr" },
	{ 0x6df8e7c3, "__brelse" },
	{ 0x57a0c51d, "kmem_cache_alloc" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0xfea22b92, "kill_block_super" },
	{ 0x94b7d62e, "fat_search_long" },
	{ 0x37500af1, "hex2bin" },
	{ 0x800fb92b, "full_name_hash" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x31a8d4ec, "fat_scan" },
	{ 0xc1fa1b01, "register_filesystem" },
	{ 0x352f82bf, "__fat_fs_error" },
	{ 0x9485496c, "iput" },
	{ 0x37a0cba, "kfree" },
	{ 0x4fcf188b, "memcpy" },
	{ 0xf75031f3, "current_time" },
	{ 0xfe84e16, "d_splice_alias" },
	{ 0x62dd0699, "fat_setattr" },
	{ 0xccaa2bf8, "fat_free_clusters" },
	{ 0x37ccd88d, "fat_get_dotdot_entry" },
	{ 0xe74da1d8, "unregister_filesystem" },
	{ 0x77ec0b14, "fat_time_unix2fat" },
	{ 0x28318305, "snprintf" },
	{ 0xaf0f157, "fat_dir_empty" },
	{ 0xfb810895, "d_instantiate" },
	{ 0xb82471e4, "clear_nlink" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=fat";

