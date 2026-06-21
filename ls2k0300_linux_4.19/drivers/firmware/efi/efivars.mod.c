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
	{ 0xaef47267, "kobject_put" },
	{ 0x33abcd4, "kset_create_and_add" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0xcdff6b94, "sysfs_remove_bin_file" },
	{ 0x968f9a23, "efivar_entry_iter_begin" },
	{ 0x98cf60b3, "strlen" },
	{ 0x85c54b61, "efivar_validate" },
	{ 0x465e24ff, "ucs2_utf8size" },
	{ 0xfed4da3e, "kobject_uevent" },
	{ 0x91715312, "sprintf" },
	{ 0xa957768f, "efivar_entry_set" },
	{ 0x9abfe05f, "efivar_entry_remove" },
	{ 0x7c32d0f0, "printk" },
	{ 0x1c3e02e4, "memcmp" },
	{ 0xfdd3bfcc, "kobject_init_and_add" },
	{ 0xad5737fc, "efivar_init" },
	{ 0x4bf1796e, "efi" },
	{ 0xd87ca6d7, "efivar_entry_find" },
	{ 0xc6cbbc89, "capable" },
	{ 0xd7399d2a, "efivar_entry_iter_end" },
	{ 0x23a2673c, "efivar_entry_get" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xa09dc25a, "kset_unregister" },
	{ 0x37a0cba, "kfree" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x38275b11, "efivar_entry_add" },
	{ 0x6f0213aa, "__efivar_entry_delete" },
	{ 0xaa047535, "sysfs_create_bin_file" },
	{ 0x5191bd3c, "efivar_work" },
	{ 0x10c029e5, "__efivar_entry_iter" },
	{ 0x93fc0ef2, "efivars_kobject" },
	{ 0x4336fcca, "ucs2_as_utf8" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "82C21A37445938E8D3B0641");
