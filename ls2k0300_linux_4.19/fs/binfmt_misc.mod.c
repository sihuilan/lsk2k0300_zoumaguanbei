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
	{ 0x20cb6a59, "kill_litter_super" },
	{ 0x81e1f87a, "noop_llseek" },
	{ 0xb5b30e0f, "default_llseek" },
	{ 0x64f1bcc, "simple_statfs" },
	{ 0xe74da1d8, "unregister_filesystem" },
	{ 0x95762403, "unregister_binfmt" },
	{ 0x1b7a2d36, "__register_binfmt" },
	{ 0xc1fa1b01, "register_filesystem" },
	{ 0x7c32d0f0, "printk" },
	{ 0x9485496c, "iput" },
	{ 0xfb810895, "d_instantiate" },
	{ 0x92f4ae9a, "simple_pin_fs" },
	{ 0xf75031f3, "current_time" },
	{ 0xe953b21f, "get_next_ino" },
	{ 0xce45ff20, "new_inode" },
	{ 0x79969bbf, "lookup_one_len" },
	{ 0x64bbc288, "string_unescape" },
	{ 0x373db350, "kstrtoint" },
	{ 0x349cba85, "strchr" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0xe6bbbe4, "up_write" },
	{ 0x6a768497, "down_write" },
	{ 0x9aa9e5f1, "simple_release_fs" },
	{ 0xc8f84617, "d_drop" },
	{ 0x39c7e78d, "drop_nlink" },
	{ 0xc528a49a, "queued_write_lock_slowpath" },
	{ 0x4302d0eb, "free_pages" },
	{ 0xd45cc6ca, "bin2hex" },
	{ 0x91715312, "sprintf" },
	{ 0x93fca811, "__get_free_pages" },
	{ 0x619cb7dd, "simple_read_from_buffer" },
	{ 0x98cf60b3, "strlen" },
	{ 0xe526f136, "__copy_user" },
	{ 0x7e58f148, "filp_close" },
	{ 0x37a0cba, "kfree" },
	{ 0xd0b8a33e, "clear_inode" },
	{ 0x25791c3d, "simple_fill_super" },
	{ 0x6731a63c, "mount_single" },
	{ 0x2097754f, "kernel_read" },
	{ 0xfb578fc5, "memset" },
	{ 0x4932e530, "search_binary_handler" },
	{ 0x9b15381e, "prepare_binprm" },
	{ 0xfbd72e50, "open_exec" },
	{ 0x617c452b, "queued_read_lock_slowpath" },
	{ 0x56351326, "would_dump" },
	{ 0xe035608d, "fd_install" },
	{ 0xa843805a, "get_unused_fd_flags" },
	{ 0xd793f804, "remove_arg_zero" },
	{ 0x13d8bd6d, "dput" },
	{ 0xd15cf93, "__close_fd" },
	{ 0xe163815b, "dentry_open" },
	{ 0xd0ed8afa, "bprm_change_interp" },
	{ 0x3cd844c4, "copy_strings_kernel" },
	{ 0x93e28ed4, "fput" },
	{ 0xd9b85ef6, "lockref_get" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x9f984513, "strrchr" },
	{ 0x11089ac7, "_ctype" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

