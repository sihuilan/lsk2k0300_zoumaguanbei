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
	{ 0x2318dc0c, "no_llseek" },
	{ 0x20ace886, "misc_deregister" },
	{ 0xac6c3ff7, "misc_register" },
	{ 0xc0a3d105, "find_next_bit" },
	{ 0x63df65bd, "input_event" },
	{ 0xe233762a, "input_event_from_user" },
	{ 0x9291cd3b, "memdup_user" },
	{ 0x925c7290, "input_ff_destroy" },
	{ 0xab0e95a2, "input_mt_init_slots" },
	{ 0x404491e2, "input_alloc_absinfo" },
	{ 0x98cf60b3, "strlen" },
	{ 0x41482d8b, "strndup_user" },
	{ 0xfffe7f76, "input_allocate_device" },
	{ 0x427bc604, "input_register_device" },
	{ 0x353449e7, "input_ff_create" },
	{ 0x4d1ff60a, "wait_for_completion_timeout" },
	{ 0x7c32d0f0, "printk" },
	{ 0xf7455c16, "input_event_to_user" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x4a928de6, "mutex_lock_interruptible" },
	{ 0xaf6ae696, "kstrndup" },
	{ 0xfb578fc5, "memset" },
	{ 0xe526f136, "__copy_user" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0x1000e51, "schedule" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x36c94dee, "input_ff_flush" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x5e515be6, "ktime_get_ts64" },
	{ 0xe23ac8b0, "nonseekable_open" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x65c9b1c7, "input_unregister_device" },
	{ 0x29361773, "complete" },
	{ 0x37a0cba, "kfree" },
	{ 0xaab455b7, "input_free_device" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

