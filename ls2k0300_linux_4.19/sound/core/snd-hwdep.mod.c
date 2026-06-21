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
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xe3d4dd66, "snd_register_device" },
	{ 0xabb7bcc2, "snd_card_file_remove" },
	{ 0x37110088, "remove_wait_queue" },
	{ 0xcb707dda, "snd_device_initialize" },
	{ 0xb2e5ae4a, "snd_lookup_minor_data" },
	{ 0x8644804e, "seq_printf" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x700b328b, "snd_ctl_unregister_ioctl" },
	{ 0xe526f136, "__copy_user" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xaad8c7d6, "default_wake_function" },
	{ 0xfb578fc5, "memset" },
	{ 0x156f81f0, "snd_device_new" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0x3cf48a2a, "snd_unregister_device" },
	{ 0x5792f848, "strlcpy" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0x8f595b11, "snd_major" },
	{ 0x33dce5f6, "_dev_err" },
	{ 0x4322b27b, "module_put" },
	{ 0x29db79b4, "snd_card_file_add" },
	{ 0xce5daa6e, "snd_ctl_register_ioctl" },
	{ 0xa61db944, "put_device" },
	{ 0x1000e51, "schedule" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x4afb2238, "add_wait_queue" },
	{ 0x37a0cba, "kfree" },
	{ 0x9de632a0, "snd_info_free_entry" },
	{ 0x6fed1cec, "dev_set_name" },
	{ 0xf07d8caf, "snd_info_create_module_entry" },
	{ 0x7222467c, "snd_info_register" },
	{ 0xa1e746d3, "try_module_get" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

