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
	{ 0x4c8c29c7, "elv_rb_latter_request" },
	{ 0x762e1f06, "elv_rb_former_request" },
	{ 0x7ecab3b, "elv_unregister" },
	{ 0xf7da213, "elv_register" },
	{ 0xcba56773, "elv_bio_merge_ok" },
	{ 0xd7c651fa, "elv_rb_find" },
	{ 0x37a0cba, "kfree" },
	{ 0xe9bd766b, "elv_dispatch_add_tail" },
	{ 0xca9360b5, "rb_next" },
	{ 0x437855e8, "elv_rb_del" },
	{ 0x526c3a6c, "jiffies" },
	{ 0xba6a77ff, "elv_rb_add" },
	{ 0xaef47267, "kobject_put" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xc0cf169, "elevator_alloc" },
	{ 0x37befc70, "jiffies_to_msecs" },
	{ 0x7f02188f, "__msecs_to_jiffies" },
	{ 0x91715312, "sprintf" },
	{ 0xb742fd7, "simple_strtol" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

