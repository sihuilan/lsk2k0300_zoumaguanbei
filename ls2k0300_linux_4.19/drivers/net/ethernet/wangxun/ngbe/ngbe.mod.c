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
	{ 0x6285be12, "device_remove_file" },
	{ 0x8f7909f4, "netdev_info" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x603c619d, "pci_write_config_dword" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0xa8717049, "loongson_dma_ops" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0xc4f0da12, "ktime_get_with_offset" },
	{ 0xf9a482f9, "msleep" },
	{ 0x862258db, "timecounter_init" },
	{ 0x31fdd04a, "dma_release_from_dev_coherent" },
	{ 0x619cb7dd, "simple_read_from_buffer" },
	{ 0x45886a0a, "pci_write_config_word" },
	{ 0x9b73726e, "debugfs_create_dir" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x2af59d26, "param_ops_int" },
	{ 0xc3755ea4, "napi_disable" },
	{ 0x98cf60b3, "strlen" },
	{ 0x4584e00c, "pci_read_config_byte" },
	{ 0xa86b8ccb, "napi_schedule_prep" },
	{ 0xcf8229ea, "__napi_schedule_irqoff" },
	{ 0xc29bf967, "strspn" },
	{ 0x43a53735, "__alloc_workqueue_key" },
	{ 0x8f996a30, "ethtool_convert_legacy_u32_to_link_mode" },
	{ 0x19f462ab, "kfree_call_rcu" },
	{ 0x83b61699, "pci_walk_bus" },
	{ 0x40b21560, "pci_disable_device" },
	{ 0xc7a4fbed, "rtnl_lock" },
	{ 0xed6abef2, "pci_disable_msix" },
	{ 0xcbf0b0d9, "hwmon_device_unregister" },
	{ 0xa381944f, "dql_reset" },
	{ 0xd0423239, "netdev_set_num_tc" },
	{ 0x1ba9cc7, "netif_carrier_on" },
	{ 0xc0a3d105, "find_next_bit" },
	{ 0xc39234d0, "netif_carrier_off" },
	{ 0x88bfa7e, "cancel_work_sync" },
	{ 0x74f0be7c, "pcie_print_link_status" },
	{ 0x5cd4fd31, "__dev_kfree_skb_any" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xf7afea6e, "pcie_capability_clear_and_set_word" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x999e8297, "vfree" },
	{ 0x62352ff4, "netdev_reset_tc" },
	{ 0x33a855cb, "debugfs_create_file" },
	{ 0x91715312, "sprintf" },
	{ 0x99fd172e, "debugfs_remove_recursive" },
	{ 0xe38d270d, "__alloc_pages_nodemask" },
	{ 0x66fead01, "netif_napi_del" },
	{ 0x526c3a6c, "jiffies" },
	{ 0x44b5ee9a, "kasprintf" },
	{ 0xc6ed130f, "__netdev_alloc_skb" },
	{ 0x4d65cbd5, "csum_ipv6_magic" },
	{ 0x6f737abf, "dma_alloc_from_dev_coherent" },
	{ 0xe8ee46da, "__pskb_pull_tail" },
	{ 0xe526f136, "__copy_user" },
	{ 0xa8d39d1c, "ptp_clock_unregister" },
	{ 0x794aef33, "pci_set_master" },
	{ 0x23d21063, "netif_schedule_queue" },
	{ 0x97934ecf, "del_timer_sync" },
	{ 0x9e3fd6fc, "_dev_warn" },
	{ 0xfb578fc5, "memset" },
	{ 0x89cf6c99, "pci_enable_pcie_error_reporting" },
	{ 0x1e1e140e, "ns_to_timespec64" },
	{ 0x964feed2, "netif_tx_wake_queue" },
	{ 0x8e5d3812, "pci_restore_state" },
	{ 0x468fd37c, "netif_tx_stop_all_queues" },
	{ 0x6c558494, "netif_set_xps_queue" },
	{ 0x7c32d0f0, "printk" },
	{ 0x414972c3, "ethtool_op_get_link" },
	{ 0x20c55ae0, "sscanf" },
	{ 0x3c3fce39, "__local_bh_enable_ip" },
	{ 0x1c3e02e4, "memcmp" },
	{ 0x3d9f21fe, "__cpu_online_mask" },
	{ 0x452ba683, "ipv6_ext_hdr" },
	{ 0xcdbf1cc, "ioremap" },
	{ 0x2be0f12d, "dql_completed" },
	{ 0xcd279169, "nla_find" },
	{ 0x5ee73171, "free_netdev" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x9166fada, "strncpy" },
	{ 0x2fe6b789, "register_netdev" },
	{ 0x5a921311, "strncmp" },
	{ 0x4e19823b, "pci_read_config_word" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0xd2c3c3d7, "netif_set_real_num_rx_queues" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x5aa09bbe, "netif_set_real_num_tx_queues" },
	{ 0xf4fbb3ec, "netif_napi_add" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0x8269d976, "ptp_clock_register" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x99189d41, "simple_open" },
	{ 0x33dce5f6, "_dev_err" },
	{ 0x537a6bb, "pci_enable_msi" },
	{ 0xeb2a37f5, "pci_clear_master" },
	{ 0x9f46ced8, "__sw_hweight64" },
	{ 0xe523ad75, "synchronize_irq" },
	{ 0x6f9e763b, "timecounter_read" },
	{ 0xb892218e, "pci_find_capability" },
	{ 0x1942cf77, "device_create_file" },
	{ 0xe0a4e6ce, "eth_get_headlen" },
	{ 0x167c5967, "print_hex_dump" },
	{ 0xcfd7791a, "pci_select_bars" },
	{ 0x55e31703, "ethtool_convert_link_mode_to_legacy_u32" },
	{ 0xb8212341, "timecounter_cyc2time" },
	{ 0xd3bbc8f5, "netif_device_attach" },
	{ 0xe3314c2c, "napi_gro_receive" },
	{ 0x2f9bd219, "_dev_info" },
	{ 0x40a9b349, "vzalloc" },
	{ 0x84262d84, "__free_pages" },
	{ 0xa8c6d1b9, "netif_device_detach" },
	{ 0x57b4ce5e, "__alloc_skb" },
	{ 0x12a38747, "usleep_range" },
	{ 0xb6efbd3f, "pci_enable_msix_range" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0xe02b673b, "pci_cleanup_aer_uncorrect_error_status" },
	{ 0xeca27aeb, "kfree_skb" },
	{ 0xd25bc5d4, "csum_tcpudp_nofold" },
	{ 0xb48f9718, "ndo_dflt_fdb_add" },
	{ 0xb907901f, "napi_complete_done" },
	{ 0xda462f1f, "pci_read_config_dword" },
	{ 0xa63a14ba, "eth_type_trans" },
	{ 0xa68a289e, "pskb_expand_head" },
	{ 0xbd50c844, "netdev_err" },
	{ 0x467df16d, "netdev_rss_key_fill" },
	{ 0x54f985aa, "pci_unregister_driver" },
	{ 0xcc5005fe, "msleep_interruptible" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xf6ebc03b, "net_ratelimit" },
	{ 0x85173a1f, "pci_set_power_state" },
	{ 0x3beffe14, "netdev_warn" },
	{ 0xbb4f4766, "simple_write_to_buffer" },
	{ 0xcd3b4b78, "eth_validate_addr" },
	{ 0x4116512e, "pci_disable_pcie_error_reporting" },
	{ 0x37a0cba, "kfree" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x433ff50d, "param_array_ops" },
	{ 0xae76f09, "ptp_clock_index" },
	{ 0x1ef8d282, "pci_disable_msi" },
	{ 0xa37755a8, "skb_add_rx_frag" },
	{ 0xe7043306, "iounmap" },
	{ 0x4a28fd1c, "pci_prepare_to_sleep" },
	{ 0x5d90053, "__pci_register_driver" },
	{ 0x15af7f4, "system_state" },
	{ 0xc3bc88e3, "request_firmware" },
	{ 0xc9807d0c, "unregister_netdev" },
	{ 0x177e5523, "ndo_dflt_bridge_getlink" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0x9e0c711d, "vzalloc_node" },
	{ 0x28318305, "snprintf" },
	{ 0xd4dfedb5, "consume_skb" },
	{ 0xf63ae5fc, "pci_enable_device_mem" },
	{ 0x516b977b, "skb_tstamp_tx" },
	{ 0x6e5606ed, "skb_put" },
	{ 0x20f0a45, "pci_wake_from_d3" },
	{ 0x23a69358, "pci_release_selected_regions" },
	{ 0x27c29128, "pci_request_selected_regions" },
	{ 0x7e1b54ee, "irq_set_affinity_hint" },
	{ 0x5f139fe4, "skb_copy_bits" },
	{ 0x27da8db4, "hwmon_device_register" },
	{ 0x79a54c7, "pci_find_ext_capability" },
	{ 0x172c7f30, "release_firmware" },
	{ 0x6e720ff2, "rtnl_unlock" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0xa507f564, "__skb_pad" },
	{ 0xe5d8b9ac, "device_set_wakeup_enable" },
	{ 0xc1514a3b, "free_irq" },
	{ 0xb11889dd, "pci_save_state" },
	{ 0xe914e41e, "strcpy" },
	{ 0x4c3f39ae, "alloc_etherdev_mqs" },
	{ 0xf6974473, "netdev_crit" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v00008088d00000000sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d00000101sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d00000102sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d00000103sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d00000104sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d00000105sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d00000106sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d00000107sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d00000108sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d00000100sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d00000109sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d0000010Bsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d0000010Asv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00008088d0000010Csv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "AFFC1047A471B428F787D4D");
