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
	{ 0x4bf1796e, "efi" },
	{ 0x94961283, "vunmap" },
	{ 0x18e9bb8a, "vmap" },
	{ 0xfb578fc5, "memset" },
	{ 0xe38d270d, "__alloc_pages_nodemask" },
	{ 0x2aadad1a, "efi_capsule_update" },
	{ 0xe526f136, "__copy_user" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x85f5e2aa, "krealloc" },
	{ 0xa5bda8a1, "efi_capsule_supported" },
	{ 0x84262d84, "__free_pages" },
	{ 0x7c32d0f0, "printk" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

