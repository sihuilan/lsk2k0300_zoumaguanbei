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
	{ 0x609f1c7e, "synchronize_net" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x497b6527, "pskb_trim_rcsum_slow" },
	{ 0x19f462ab, "kfree_call_rcu" },
	{ 0x10fd167, "skb_clone" },
	{ 0xe8ee46da, "__pskb_pull_tail" },
	{ 0x7c32d0f0, "printk" },
	{ 0x3c3fce39, "__local_bh_enable_ip" },
	{ 0x8b8890f1, "skb_push" },
	{ 0x7a7ce417, "dev_remove_pack" },
	{ 0xd441ebc1, "skb_pull" },
	{ 0x4474e925, "init_net" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0xeca27aeb, "kfree_skb" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xdcb90172, "dev_add_pack" },
	{ 0xd4dfedb5, "consume_skb" },
	{ 0xc122a89a, "dev_queue_xmit" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

