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
	{ 0x9c38cd9f, "kmem_cache_destroy" },
	{ 0xaa68ccfa, "iget_failed" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0xbd829c21, "sb_min_blocksize" },
	{ 0x78470c5, "generic_file_llseek" },
	{ 0x60a13e90, "rcu_barrier" },
	{ 0x659b7c5c, "iget5_locked" },
	{ 0xec7bc422, "from_kuid_munged" },
	{ 0x75063ef6, "seq_puts" },
	{ 0x8644804e, "seq_printf" },
	{ 0x44e9a829, "match_token" },
	{ 0x6634eaae, "init_user_ns" },
	{ 0xfb2f8a4, "mktime64" },
	{ 0x1bbfa54e, "mount_bdev" },
	{ 0x85df9b6c, "strsep" },
	{ 0xbd460891, "page_symlink_inode_operations" },
	{ 0x8e2d3ae9, "generic_read_dir" },
	{ 0x75132049, "__getblk_gfp" },
	{ 0x91715312, "sprintf" },
	{ 0xe38d270d, "__alloc_pages_nodemask" },
	{ 0x82f169fa, "from_kgid_munged" },
	{ 0x9d0e7cf1, "make_kgid" },
	{ 0xfd5142d1, "mpage_readpages" },
	{ 0xfb578fc5, "memset" },
	{ 0x67db6d90, "mpage_readpage" },
	{ 0x11089ac7, "_ctype" },
	{ 0x59ac3e11, "__bread_gfp" },
	{ 0x7c32d0f0, "printk" },
	{ 0xa9e3bd89, "d_obtain_alias" },
	{ 0x1c3e02e4, "memcmp" },
	{ 0x5a921311, "strncmp" },
	{ 0xe5a21497, "kmem_cache_free" },
	{ 0x8bba9570, "set_nlink" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0xa07a37f0, "memchr" },
	{ 0x62597d, "generic_ro_fops" },
	{ 0x4e3567f7, "match_int" },
	{ 0x8514d484, "unlock_page" },
	{ 0xb19a328e, "inode_nohighmem" },
	{ 0x6df8e7c3, "__brelse" },
	{ 0xc136b8a3, "inode_init_once" },
	{ 0x57a0c51d, "kmem_cache_alloc" },
	{ 0x84262d84, "__free_pages" },
	{ 0xbc601b3, "make_kuid" },
	{ 0x93fca811, "__get_free_pages" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x96b29254, "strncasecmp" },
	{ 0x604b69fb, "unlock_new_inode" },
	{ 0xfea22b92, "kill_block_super" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xfece5f82, "kmem_cache_create" },
	{ 0xc1fa1b01, "register_filesystem" },
	{ 0x4302d0eb, "free_pages" },
	{ 0x9485496c, "iput" },
	{ 0x37a0cba, "kfree" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x643e0ce5, "call_rcu_sched" },
	{ 0xfe84e16, "d_splice_alias" },
	{ 0x752bb08e, "sync_filesystem" },
	{ 0xb65681c, "sb_set_blocksize" },
	{ 0xb76e5e5c, "d_make_root" },
	{ 0x3cbde74b, "ioctl_by_bdev" },
	{ 0xe74da1d8, "unregister_filesystem" },
	{ 0x57f44828, "init_special_inode" },
	{ 0x662c0ae6, "generic_block_bmap" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

