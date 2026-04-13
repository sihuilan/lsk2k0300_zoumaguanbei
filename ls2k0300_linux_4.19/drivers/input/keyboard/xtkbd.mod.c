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
	{ 0x3f33483e, "serio_unregister_driver" },
	{ 0xc7127fe7, "__serio_register_driver" },
	{ 0x7c32d0f0, "printk" },
	{ 0x63df65bd, "input_event" },
	{ 0x427bc604, "input_register_device" },
	{ 0xaab455b7, "input_free_device" },
	{ 0x7ef57684, "serio_open" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x28318305, "snprintf" },
	{ 0xfffe7f76, "input_allocate_device" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
	{ 0x65c9b1c7, "input_unregister_device" },
	{ 0x2723cb34, "serio_close" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("serio:ty00pr*id*ex*");
