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
	{ 0x8f7909f4, "netdev_info" },
	{ 0x60365e6b, "register_candev" },
	{ 0xa0f163f5, "alloc_can_err_skb" },
	{ 0x8b66c3db, "can_bus_off" },
	{ 0x102686af, "netif_rx" },
	{ 0x6ea5b366, "close_candev" },
	{ 0x964feed2, "netif_tx_wake_queue" },
	{ 0x7c32d0f0, "printk" },
	{ 0x4b1444be, "alloc_candev_mqs" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x7b0cdbb0, "free_candev" },
	{ 0xa7b36e02, "can_change_mtu" },
	{ 0x70137bb6, "can_change_state" },
	{ 0x3cfbf977, "unregister_candev" },
	{ 0xa3e2b52, "alloc_can_skb" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0xeca27aeb, "kfree_skb" },
	{ 0xbd50c844, "netdev_err" },
	{ 0xb94c215d, "open_candev" },
	{ 0x3beffe14, "netdev_warn" },
	{ 0x68bc8859, "can_get_echo_skb" },
	{ 0x698c7d91, "can_put_echo_skb" },
	{ 0x17af6d7b, "can_free_echo_skb" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0xc1514a3b, "free_irq" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

