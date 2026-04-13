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
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0xe93e49c3, "devres_free" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xf6c1256d, "cancel_delayed_work_sync" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x91715312, "sprintf" },
	{ 0xe949e6e8, "devres_alloc_node" },
	{ 0x7c32d0f0, "printk" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0xd4034828, "system_freezable_wq" },
	{ 0xa61db944, "put_device" },
	{ 0x164e7c53, "queue_delayed_work_on" },
	{ 0x427bc604, "input_register_device" },
	{ 0xe6618e89, "devres_add" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x6901d423, "get_device" },
	{ 0x1eb9516e, "round_jiffies_relative" },
	{ 0x37a0cba, "kfree" },
	{ 0xa46f2f1b, "kstrtouint" },
	{ 0x65c9b1c7, "input_unregister_device" },
	{ 0xd0b56f0c, "devres_destroy" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0xfffe7f76, "input_allocate_device" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

