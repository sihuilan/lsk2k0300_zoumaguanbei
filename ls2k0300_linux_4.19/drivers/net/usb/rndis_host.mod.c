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
	{ 0xf9a482f9, "msleep" },
	{ 0x2b5b7b89, "usbnet_resume" },
	{ 0x47f6dc9d, "usbnet_probe" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0x6631f31c, "usbnet_cdc_unbind" },
	{ 0x61008ae5, "usbnet_disconnect" },
	{ 0x10fd167, "skb_clone" },
	{ 0x88c7f638, "usbnet_generic_cdc_bind" },
	{ 0x5cd4fd31, "__dev_kfree_skb_any" },
	{ 0x90bc9822, "usbnet_stop" },
	{ 0x6212a765, "skb_trim" },
	{ 0x9e3fd6fc, "_dev_warn" },
	{ 0xfb578fc5, "memset" },
	{ 0x635e4f9b, "usbnet_get_stats64" },
	{ 0x5d5839a, "usb_deregister" },
	{ 0x7c32d0f0, "printk" },
	{ 0x206392a8, "usb_control_msg" },
	{ 0x6597360a, "usbnet_start_xmit" },
	{ 0x828b1c15, "usbnet_suspend" },
	{ 0xd441ebc1, "skb_pull" },
	{ 0x33dce5f6, "_dev_err" },
	{ 0xf894dedd, "skb_copy_expand" },
	{ 0x2f9bd219, "_dev_info" },
	{ 0xb37aaded, "usbnet_tx_timeout" },
	{ 0xe5eef155, "usbnet_skb_return" },
	{ 0x7f239a1e, "usb_driver_release_interface" },
	{ 0x2756c75b, "usbnet_open" },
	{ 0xa708864, "usb_interrupt_msg" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xcd3b4b78, "eth_validate_addr" },
	{ 0x37a0cba, "kfree" },
	{ 0x141ead0c, "usb_register_driver" },
	{ 0xbe66560, "memmove" },
	{ 0x16217bea, "eth_mac_addr" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=usbnet,cdc_ether";

MODULE_ALIAS("usb:v1630p0042d*dc*dsc*dp*ic02isc02ipFFin*");
MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*ic02isc02ipFFin*");
MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*icEFisc01ip01in*");
MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*icE0isc01ip03in*");
MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*icEFisc04ip01in*");
