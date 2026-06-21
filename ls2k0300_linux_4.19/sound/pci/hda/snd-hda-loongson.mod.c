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
	{ 0x2d3385d3, "system_wq" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xa8717049, "loongson_dma_ops" },
	{ 0x38ce402b, "azx_interrupt" },
	{ 0x34360a65, "param_get_int" },
	{ 0x2af59d26, "param_ops_int" },
	{ 0x3fef370, "snd_hdac_bus_alloc_stream_pages" },
	{ 0x45a52213, "snd_pcm_period_elapsed" },
	{ 0xc19799e1, "snd_dma_alloc_pages" },
	{ 0x67257a08, "azx_codec_configure" },
	{ 0x88bfa7e, "cancel_work_sync" },
	{ 0xb898a5c9, "param_ops_bool" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x82765a3b, "param_set_int" },
	{ 0x57ff31d5, "pm_runtime_forbid" },
	{ 0x525984b8, "azx_stop_chip" },
	{ 0xaff0848a, "snd_hda_set_power_save" },
	{ 0xf70484b8, "snd_card_disconnect" },
	{ 0x6f8c9a67, "__platform_driver_register" },
	{ 0xfd94814e, "complete_all" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x3ecc865c, "param_ops_charp" },
	{ 0x156f81f0, "snd_device_new" },
	{ 0x4beb6814, "azx_init_chip" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0x7c32d0f0, "printk" },
	{ 0xcdbf1cc, "ioremap" },
	{ 0x9b1e5f44, "azx_free_streams" },
	{ 0x688ba003, "azx_bus_init" },
	{ 0x5792f848, "strlcpy" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0xfaaf19ba, "snd_pcm_lib_free_pages" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x33dce5f6, "_dev_err" },
	{ 0x35a56bf4, "snd_pcm_lib_malloc_pages" },
	{ 0xe523ad75, "synchronize_irq" },
	{ 0x1e2a087b, "param_ops_bint" },
	{ 0x33afca5d, "snd_card_new" },
	{ 0x2f9bd219, "_dev_info" },
	{ 0x164e7c53, "queue_delayed_work_on" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x43cb4fd5, "azx_get_pos_lpib" },
	{ 0x9d5ee641, "snd_hdac_bus_exit" },
	{ 0xd26bf1f7, "snd_hdac_bus_free_stream_pages" },
	{ 0x4b1a6fdb, "__pm_runtime_set_status" },
	{ 0x3c037d47, "snd_dma_free_pages" },
	{ 0x126cd7a9, "azx_stop_all_streams" },
	{ 0x54f985aa, "pci_unregister_driver" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x37a0cba, "kfree" },
	{ 0x433ff50d, "param_array_ops" },
	{ 0xe7043306, "iounmap" },
	{ 0x5d90053, "__pci_register_driver" },
	{ 0xe1fbef6c, "azx_init_streams" },
	{ 0x2d1c65c9, "snd_card_free" },
	{ 0xb2d90b72, "snd_card_register" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0x28318305, "snprintf" },
	{ 0x6866da8c, "azx_probe_codecs" },
	{ 0x64c9b254, "platform_get_irq" },
	{ 0x366ed466, "snd_hdac_bus_enter_link_reset" },
	{ 0x4014357a, "platform_driver_unregister" },
	{ 0x58475508, "pci_enable_device" },
	{ 0x4cd7169c, "pci_request_region" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xe914e41e, "strcpy" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=snd-hda-codec,snd-hda-core";

MODULE_ALIAS("pci:v00000014d00007A37sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00000014d00007A07sv*sd*bc*sc*i*");
MODULE_ALIAS("of:N*T*Cloongson,ls2h-audio");
MODULE_ALIAS("of:N*T*Cloongson,ls2h-audioC*");
MODULE_ALIAS("of:N*T*Cloongson,ls2k-audio");
MODULE_ALIAS("of:N*T*Cloongson,ls2k-audioC*");
