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
	{ 0x5d5839a, "usb_deregister" },
	{ 0x8c03d20c, "destroy_workqueue" },
	{ 0x141ead0c, "usb_register_driver" },
	{ 0x43a53735, "__alloc_workqueue_key" },
	{ 0x1bc3edc2, "usb_stor_sense_invalidCDB" },
	{ 0xce7da670, "scsi_cmd_get_serial" },
	{ 0x9cb5b641, "sdev_prefix_printk" },
	{ 0x9c562ee6, "usb_unlink_urb" },
	{ 0x84aacff, "usb_unanchor_urb" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x5ed90adc, "int_to_scsilun" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x9d49ca67, "usb_submit_urb" },
	{ 0x5815708f, "usb_anchor_urb" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xdde76930, "usb_alloc_urb" },
	{ 0x9e3fd6fc, "_dev_warn" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0x2d3385d3, "system_wq" },
	{ 0x91139bd0, "scsi_add_host_with_dma" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xfb578fc5, "memset" },
	{ 0xd243c7f8, "scsi_host_alloc" },
	{ 0x86cb4595, "usb_stor_adjust_quirks" },
	{ 0xe64ddd3c, "blk_queue_max_hw_sectors" },
	{ 0xd654ea14, "blk_queue_update_dma_alignment" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x16e3a597, "usb_lock_device_for_reset" },
	{ 0x2f34c81c, "scsi_host_put" },
	{ 0xdb01e922, "scsi_remove_host" },
	{ 0x962c8ae1, "usb_kill_anchored_urbs" },
	{ 0x88bfa7e, "cancel_work_sync" },
	{ 0x7c32d0f0, "printk" },
	{ 0xf5ec4073, "scsi_report_bus_reset" },
	{ 0x8d86d7e9, "usb_alloc_streams" },
	{ 0xced7e9b3, "usb_get_urb" },
	{ 0xa693e4b3, "usb_kill_urb" },
	{ 0xb775e3da, "scsi_change_queue_depth" },
	{ 0x2cfb85c0, "scsi_is_host_device" },
	{ 0x33dce5f6, "_dev_err" },
	{ 0xec10ce17, "scsi_scan_host" },
	{ 0xda9dc303, "usb_free_urb" },
	{ 0xabd8d4bb, "scsi_print_command" },
	{ 0x5a410e76, "scmd_printk" },
	{ 0xd8001816, "scsi_unblock_requests" },
	{ 0xeedb9e84, "dev_printk" },
	{ 0x7f2b642f, "scsi_block_requests" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x407af304, "usb_wait_anchor_empty_timeout" },
	{ 0x8487a2b6, "flush_work" },
	{ 0x526c3a6c, "jiffies" },
	{ 0x5c59a78d, "usb_reset_device" },
	{ 0xaed47d9d, "usb_set_interface" },
	{ 0x15af7f4, "system_state" },
	{ 0x9b0cf0f3, "usb_free_streams" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("usb:v054Cp087Dd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v059Fp105Fd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0984p0301d0128dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v0BC2p331Ad*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v13FDp3940d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v152Dp0539d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v152Dp0567d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v152Dp0578d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v154BpF00Bd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v154BpF00Dd*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2109p0711d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v2537p1068d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v357Dp7788d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v4971p1012d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v4971p8017d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v4971p8024d*dc*dsc*dp*ic*isc*ip*in*");
MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*ic08isc06ip50in*");
MODULE_ALIAS("usb:v*p*d*dc*dsc*dp*ic08isc06ip62in*");
