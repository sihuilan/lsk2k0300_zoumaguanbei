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
	{ 0x42595e58, "vgacon_text_force" },
	{ 0x44cc7d43, "drm_release" },
	{ 0x657c18ce, "drm_fb_helper_set_par" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x3c5dc2c3, "ttm_bo_eviction_valuable" },
	{ 0x603c619d, "pci_write_config_dword" },
	{ 0xc647ef1a, "ttm_tt_fini" },
	{ 0xf9a482f9, "msleep" },
	{ 0x198ddd09, "drm_crtc_helper_set_config" },
	{ 0xfa3860dc, "drm_helper_resume_force_mode" },
	{ 0x2012ee7, "drm_get_edid" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x6795595a, "drm_fb_helper_initial_config" },
	{ 0x2af59d26, "param_ops_int" },
	{ 0x40077784, "drm_crtc_force_disable_all" },
	{ 0x380be0b3, "drm_mode_config_cleanup" },
	{ 0xbb4e5578, "drm_mode_config_reset" },
	{ 0x40b21560, "pci_disable_device" },
	{ 0x4957585e, "ttm_bo_validate" },
	{ 0x2b8a4d97, "ttm_bo_init" },
	{ 0xb702acc, "drm_fb_helper_debug_leave" },
	{ 0x4252b1c3, "drm_helper_probe_single_connector_modes" },
	{ 0x5d7318dc, "drm_global_item_ref" },
	{ 0x999e8297, "vfree" },
	{ 0x4e0f1874, "drm_encoder_init" },
	{ 0x94fe9bfa, "drm_connector_cleanup" },
	{ 0xb3b42a38, "mutex_trylock" },
	{ 0x452674b5, "drm_fb_helper_unregister_fbi" },
	{ 0xc631580a, "console_unlock" },
	{ 0xad4c780f, "drm_fb_helper_pan_display" },
	{ 0x1a578d97, "ttm_mem_global_init" },
	{ 0xe2e97021, "drm_connector_update_edid_property" },
	{ 0x5d7c958a, "drm_helper_connector_dpms" },
	{ 0x4ef4744, "drm_fb_helper_set_suspend" },
	{ 0x8cabce28, "ttm_mem_global_release" },
	{ 0x3e07f6e6, "ttm_bo_unref" },
	{ 0xb9056bb6, "remove_conflicting_framebuffers" },
	{ 0xa3419e18, "ttm_bo_add_to_lru" },
	{ 0x6e16cc30, "drm_helper_mode_fill_fb_struct" },
	{ 0xf77555cd, "__memcpy_toio" },
	{ 0x7c32d0f0, "printk" },
	{ 0x7d38e0c2, "drm_put_dev" },
	{ 0x22983638, "drm_gem_handle_create" },
	{ 0x35ee4832, "drm_gem_object_put_unlocked" },
	{ 0x2bcca553, "ttm_bo_kmap" },
	{ 0x6bf2adb9, "drm_fb_helper_fill_var" },
	{ 0x182c2a43, "ttm_tt_init" },
	{ 0xfbaaf01e, "console_lock" },
	{ 0xe1d54f8f, "drm_gem_object_release" },
	{ 0xf551962e, "drm_read" },
	{ 0xc597539c, "ttm_bo_dma_acc_size" },
	{ 0x2cba18f7, "drm_add_edid_modes" },
	{ 0x7c2ee37d, "ttm_bo_init_mm" },
	{ 0x336f37a9, "ttm_bo_device_init" },
	{ 0x1dd71f7a, "drm_err" },
	{ 0x3df06ea4, "drm_connector_init" },
	{ 0x9ae3fa44, "drm_kms_helper_poll_enable" },
	{ 0x39b1e1d9, "drm_fb_helper_prepare" },
	{ 0xb4017d62, "i2c_del_adapter" },
	{ 0xd7c5a484, "drm_ioctl" },
	{ 0x2ed95f62, "drm_fb_helper_sys_imageblit" },
	{ 0xe8b5d764, "drm_framebuffer_unregister_private" },
	{ 0xa8a72613, "ttm_bo_del_sub_from_lru" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x9e9018ae, "drm_fb_helper_check_var" },
	{ 0x3b24dadb, "drm_fb_helper_blank" },
	{ 0xdbf6c0ed, "ttm_bo_kunmap" },
	{ 0xd2a62f13, "drm_get_pci_dev" },
	{ 0xda462f1f, "pci_read_config_dword" },
	{ 0xe4ae6d6a, "drm_connector_attach_encoder" },
	{ 0xc669aaeb, "drm_gem_object_lookup" },
	{ 0x54f985aa, "pci_unregister_driver" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x77baaa60, "drm_fb_helper_alloc_fbi" },
	{ 0x1a17700e, "drm_crtc_init" },
	{ 0xdac5bf6e, "drm_connector_unregister" },
	{ 0x3d9f2df1, "drm_gem_object_init" },
	{ 0x94c54bd, "drm_encoder_cleanup" },
	{ 0x85173a1f, "pci_set_power_state" },
	{ 0x8b05bd0, "ttm_bo_device_release" },
	{ 0xe7c43b93, "drm_crtc_cleanup" },
	{ 0xd421ce45, "drm_mode_config_init" },
	{ 0x37a0cba, "kfree" },
	{ 0x9765a123, "ww_mutex_unlock" },
	{ 0x5d90053, "__pci_register_driver" },
	{ 0xc3bc88e3, "request_firmware" },
	{ 0x5299c287, "drm_fb_helper_fini" },
	{ 0x2109a78c, "drm_dbg" },
	{ 0x6b0171a6, "i2c_bit_add_bus" },
	{ 0x92eddb0a, "drm_fb_helper_sys_fillrect" },
	{ 0x564e1aee, "ttm_bo_global_init" },
	{ 0x28318305, "snprintf" },
	{ 0x4b78848c, "ttm_bo_manager_func" },
	{ 0x5827525f, "drm_helper_disable_unused_functions" },
	{ 0xc9aa5c5a, "pci_iomap" },
	{ 0x570a1da0, "drm_fb_helper_setcmap" },
	{ 0x97634aef, "drm_framebuffer_init" },
	{ 0xd4735d6b, "drm_vma_node_is_allowed" },
	{ 0xbaf460f9, "of_property_read_variable_u32_array" },
	{ 0x58475508, "pci_enable_device" },
	{ 0xbf8390f, "drm_fb_helper_sys_copyarea" },
	{ 0x52e3775e, "drm_fb_helper_fill_fix" },
	{ 0x2935cda, "drm_connector_register" },
	{ 0x29f078d1, "drm_mode_legacy_fb_format" },
	{ 0xb0c6ef4d, "drm_mode_crtc_set_gamma_size" },
	{ 0x1a42557f, "drm_kms_helper_poll_disable" },
	{ 0xa0e6f517, "drm_mode_object_find" },
	{ 0x4e8dd264, "ttm_bo_global_release" },
	{ 0x172c7f30, "release_firmware" },
	{ 0xbe3ad498, "ww_mutex_lock_interruptible" },
	{ 0x66e9eee6, "drm_fb_helper_debug_enter" },
	{ 0x8f264633, "drm_poll" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0x459fba9a, "drm_fb_helper_init" },
	{ 0x8a3796a2, "drm_fb_helper_single_add_all_connectors" },
	{ 0xb2eabfb9, "drm_mode_vrefresh" },
	{ 0x42c4dee8, "drm_open" },
	{ 0xb11889dd, "pci_save_state" },
	{ 0xe914e41e, "strcpy" },
	{ 0xb5540008, "drm_global_item_unref" },
	{ 0x114146d, "drm_framebuffer_cleanup" },
	{ 0x7e170611, "ttm_bo_mmap" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v00001A03d00002000sv*sd*bc03sc*i*");
MODULE_ALIAS("pci:v00001A03d00002010sv*sd*bc03sc*i*");
