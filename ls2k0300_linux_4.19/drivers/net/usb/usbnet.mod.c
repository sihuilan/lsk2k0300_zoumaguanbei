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
	{ 0x8f7909f4, "netdev_info" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x5e043902, "ethtool_op_get_ts_info" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0x50b03c47, "sg_init_table" },
	{ 0xad995dac, "netdev_stats_to_stats64" },
	{ 0x2af59d26, "param_ops_int" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0x37110088, "remove_wait_queue" },
	{ 0x1ba9cc7, "netif_carrier_on" },
	{ 0x2b769473, "usb_get_from_anchor" },
	{ 0xc39234d0, "netif_carrier_off" },
	{ 0x88bfa7e, "cancel_work_sync" },
	{ 0xa693e4b3, "usb_kill_urb" },
	{ 0x807766ea, "usb_scuttle_anchored_urbs" },
	{ 0x448eac3e, "kmemdup" },
	{ 0x5cd4fd31, "__dev_kfree_skb_any" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x610b83b6, "usb_autopm_get_interface" },
	{ 0xced7e9b3, "usb_get_urb" },
	{ 0x9c562ee6, "usb_unlink_urb" },
	{ 0x526c3a6c, "jiffies" },
	{ 0xc9ec4e21, "free_percpu" },
	{ 0x15e8e1d9, "cpumask_next" },
	{ 0xc6ed130f, "__netdev_alloc_skb" },
	{ 0x102686af, "netif_rx" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xaad8c7d6, "default_wake_function" },
	{ 0x17de3d5, "nr_cpu_ids" },
	{ 0x97934ecf, "del_timer_sync" },
	{ 0x94e6be20, "skb_queue_purge" },
	{ 0xfb578fc5, "memset" },
	{ 0xbb7a8d8e, "__cpu_possible_mask" },
	{ 0xa93b4e21, "usb_string" },
	{ 0x964feed2, "netif_tx_wake_queue" },
	{ 0x295b1aff, "mii_nway_restart" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0x7c32d0f0, "printk" },
	{ 0x414972c3, "ethtool_op_get_link" },
	{ 0xaed47d9d, "usb_set_interface" },
	{ 0x5ee73171, "free_netdev" },
	{ 0x5b72900f, "usb_autopm_put_interface_async" },
	{ 0xfaef0ed, "__tasklet_schedule" },
	{ 0x2fe6b789, "register_netdev" },
	{ 0x206392a8, "usb_control_msg" },
	{ 0x5792f848, "strlcpy" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0x82072614, "tasklet_kill" },
	{ 0xbd671048, "__alloc_percpu_gfp" },
	{ 0x735980da, "mii_ethtool_set_link_ksettings" },
	{ 0xac0a556e, "skb_queue_tail" },
	{ 0xd3bbc8f5, "netif_device_attach" },
	{ 0x9d49ca67, "usb_submit_urb" },
	{ 0xa8c6d1b9, "netif_device_detach" },
	{ 0xfdbb08bb, "usb_autopm_get_interface_async" },
	{ 0xf0097b24, "pm_runtime_enable" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0x7e72a9d8, "usb_clear_halt" },
	{ 0xa63a14ba, "eth_type_trans" },
	{ 0x37500af1, "hex2bin" },
	{ 0xbd50c844, "netdev_err" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x1176e07f, "__per_cpu_offset" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x64c6d948, "mii_ethtool_get_link_ksettings" },
	{ 0xfabd0184, "usb_autopm_get_interface_no_resume" },
	{ 0xcd3b4b78, "eth_validate_addr" },
	{ 0x4afb2238, "add_wait_queue" },
	{ 0x37a0cba, "kfree" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x6bebeeee, "skb_dequeue" },
	{ 0xc9807d0c, "unregister_netdev" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0x28318305, "snprintf" },
	{ 0xc6583e04, "mii_link_ok" },
	{ 0xd4dfedb5, "consume_skb" },
	{ 0x516b977b, "skb_tstamp_tx" },
	{ 0x6e5606ed, "skb_put" },
	{ 0x16217bea, "eth_mac_addr" },
	{ 0xda9dc303, "usb_free_urb" },
	{ 0xd9c5db9, "usb_autopm_put_interface" },
	{ 0x5815708f, "usb_anchor_urb" },
	{ 0xdde76930, "usb_alloc_urb" },
	{ 0xe914e41e, "strcpy" },
	{ 0x4c3f39ae, "alloc_etherdev_mqs" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

