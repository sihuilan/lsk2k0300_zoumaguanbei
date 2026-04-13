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
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0xa8717049, "loongson_dma_ops" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0x31fdd04a, "dma_release_from_dev_coherent" },
	{ 0x364adcc9, "pcim_enable_device" },
	{ 0x2af59d26, "param_ops_int" },
	{ 0x91eb9b4, "round_jiffies" },
	{ 0xc3755ea4, "napi_disable" },
	{ 0xa86b8ccb, "napi_schedule_prep" },
	{ 0xcf8229ea, "__napi_schedule_irqoff" },
	{ 0x8f996a30, "ethtool_convert_legacy_u32_to_link_mode" },
	{ 0x19f462ab, "kfree_call_rcu" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0x40b21560, "pci_disable_device" },
	{ 0x81047dba, "hrtimer_cancel" },
	{ 0xf3936912, "pcim_iomap" },
	{ 0xc7a4fbed, "rtnl_lock" },
	{ 0xed6abef2, "pci_disable_msix" },
	{ 0xa381944f, "dql_reset" },
	{ 0x1ba9cc7, "netif_carrier_on" },
	{ 0xc0a3d105, "find_next_bit" },
	{ 0xc39234d0, "netif_carrier_off" },
	{ 0xad27f361, "__warn_printk" },
	{ 0x88bfa7e, "cancel_work_sync" },
	{ 0xf88a6f7b, "call_netdevice_notifiers" },
	{ 0x5cd4fd31, "__dev_kfree_skb_any" },
	{ 0x6c438687, "pci_release_regions" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x999e8297, "vfree" },
	{ 0xdd64e639, "strscpy" },
	{ 0x91715312, "sprintf" },
	{ 0xe38d270d, "__alloc_pages_nodemask" },
	{ 0x66fead01, "netif_napi_del" },
	{ 0x526c3a6c, "jiffies" },
	{ 0xc6ed130f, "__netdev_alloc_skb" },
	{ 0x102686af, "netif_rx" },
	{ 0x6f737abf, "dma_alloc_from_dev_coherent" },
	{ 0x794aef33, "pci_set_master" },
	{ 0x23d21063, "netif_schedule_queue" },
	{ 0x97934ecf, "del_timer_sync" },
	{ 0x54e2888, "hrtimer_start_range_ns" },
	{ 0xfb578fc5, "memset" },
	{ 0x964feed2, "netif_tx_wake_queue" },
	{ 0x8e5d3812, "pci_restore_state" },
	{ 0x468fd37c, "netif_tx_stop_all_queues" },
	{ 0x7c32d0f0, "printk" },
	{ 0x414972c3, "ethtool_op_get_link" },
	{ 0x3c3fce39, "__local_bh_enable_ip" },
	{ 0x1c3e02e4, "memcmp" },
	{ 0x3d9f21fe, "__cpu_online_mask" },
	{ 0x2be0f12d, "dql_completed" },
	{ 0x5ee73171, "free_netdev" },
	{ 0x9166fada, "strncpy" },
	{ 0x2fe6b789, "register_netdev" },
	{ 0x5792f848, "strlcpy" },
	{ 0xfe683b67, "netdev_printk" },
	{ 0xd2c3c3d7, "netif_set_real_num_rx_queues" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x5aa09bbe, "netif_set_real_num_tx_queues" },
	{ 0xf4fbb3ec, "netif_napi_add" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x33dce5f6, "_dev_err" },
	{ 0x9f46ced8, "__sw_hweight64" },
	{ 0xe523ad75, "synchronize_irq" },
	{ 0xcfd7791a, "pci_select_bars" },
	{ 0x55e31703, "ethtool_convert_link_mode_to_legacy_u32" },
	{ 0xd3bbc8f5, "netif_device_attach" },
	{ 0xe3314c2c, "napi_gro_receive" },
	{ 0x2f9bd219, "_dev_info" },
	{ 0x40a9b349, "vzalloc" },
	{ 0x84262d84, "__free_pages" },
	{ 0xa8c6d1b9, "netif_device_detach" },
	{ 0x12a38747, "usleep_range" },
	{ 0xb6efbd3f, "pci_enable_msix_range" },
	{ 0x5e3290d3, "ipv6_skip_exthdr" },
	{ 0x6fa3ae6a, "__napi_schedule" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0xdc9510bf, "skb_checksum_help" },
	{ 0xb907901f, "napi_complete_done" },
	{ 0xda462f1f, "pci_read_config_dword" },
	{ 0xa63a14ba, "eth_type_trans" },
	{ 0xa68a289e, "pskb_expand_head" },
	{ 0x54f985aa, "pci_unregister_driver" },
	{ 0x85173a1f, "pci_set_power_state" },
	{ 0x37a0cba, "kfree" },
	{ 0x4fcf188b, "memcpy" },
	{ 0xa37755a8, "skb_add_rx_frag" },
	{ 0xcff83999, "hrtimer_init" },
	{ 0x5d90053, "__pci_register_driver" },
	{ 0xc9807d0c, "unregister_netdev" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0x9e0c711d, "vzalloc_node" },
	{ 0x28318305, "snprintf" },
	{ 0xd4dfedb5, "consume_skb" },
	{ 0xf63ae5fc, "pci_enable_device_mem" },
	{ 0x23a69358, "pci_release_selected_regions" },
	{ 0x27c29128, "pci_request_selected_regions" },
	{ 0x7e1b54ee, "irq_set_affinity_hint" },
	{ 0xccea43e9, "param_ops_uint" },
	{ 0x5f139fe4, "skb_copy_bits" },
	{ 0x79a54c7, "pci_find_ext_capability" },
	{ 0x6e720ff2, "rtnl_unlock" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0xa507f564, "__skb_pad" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xb11889dd, "pci_save_state" },
	{ 0x4c3f39ae, "alloc_etherdev_mqs" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v00008848d00001080sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008848d00001081sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "368A5A5B1361293A2E9A283");
