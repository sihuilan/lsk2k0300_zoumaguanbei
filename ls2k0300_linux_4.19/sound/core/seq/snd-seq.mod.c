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
	{ 0x402b8281, "__request_module" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x6847bcc5, "put_pid" },
	{ 0xdb38c297, "up_read" },
	{ 0xe3d4dd66, "snd_register_device" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x2af59d26, "param_ops_int" },
	{ 0x151f4898, "schedule_timeout_uninterruptible" },
	{ 0x94e126a, "pid_vnr" },
	{ 0x37110088, "remove_wait_queue" },
	{ 0xcb707dda, "snd_device_initialize" },
	{ 0x2318dc0c, "no_llseek" },
	{ 0x6339b6d0, "snd_seq_device_load_drivers" },
	{ 0x8644804e, "seq_printf" },
	{ 0xad27f361, "__warn_printk" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x999e8297, "vfree" },
	{ 0xdd64e639, "strscpy" },
	{ 0x91715312, "sprintf" },
	{ 0xe23ac8b0, "nonseekable_open" },
	{ 0xadaf718e, "down_read" },
	{ 0xe526f136, "__copy_user" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xaad8c7d6, "default_wake_function" },
	{ 0x91def1c, "snd_seq_autoload_exit" },
	{ 0xfb578fc5, "memset" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0x7c32d0f0, "printk" },
	{ 0x3cf48a2a, "snd_unregister_device" },
	{ 0x3971b4df, "snd_ecards_limit" },
	{ 0x365acda7, "set_normalized_timespec64" },
	{ 0x5a921311, "strncmp" },
	{ 0x5792f848, "strlcpy" },
	{ 0x8c1388f2, "snd_timer_pause" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0xf201bc84, "snd_timer_resolution" },
	{ 0xe6bbbe4, "up_write" },
	{ 0x6a768497, "down_write" },
	{ 0x4322b27b, "module_put" },
	{ 0x477281db, "snd_timer_start" },
	{ 0xa61db944, "put_device" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x617c452b, "queued_read_lock_slowpath" },
	{ 0x1000e51, "schedule" },
	{ 0xb4e9a14e, "snd_timer_open" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x1e01660e, "vsnprintf" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x4afb2238, "add_wait_queue" },
	{ 0x37a0cba, "kfree" },
	{ 0x4fcf188b, "memcpy" },
	{ 0xe8bf72e1, "snd_timer_close" },
	{ 0x86ed32c4, "snd_seq_root" },
	{ 0x9de632a0, "snd_info_free_entry" },
	{ 0x433ff50d, "param_array_ops" },
	{ 0x4a3ea5c0, "snd_request_card" },
	{ 0x28318305, "snprintf" },
	{ 0x6fed1cec, "dev_set_name" },
	{ 0x5e515be6, "ktime_get_ts64" },
	{ 0x370a0736, "snd_seq_autoload_init" },
	{ 0xf07d8caf, "snd_info_create_module_entry" },
	{ 0x7222467c, "snd_info_register" },
	{ 0x4edd04ae, "__init_rwsem" },
	{ 0xa1e746d3, "try_module_get" },
	{ 0xe914e41e, "strcpy" },
	{ 0xc528a49a, "queued_write_lock_slowpath" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=snd-seq-device";

