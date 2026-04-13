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
	{ 0x8872189e, "vb2_ioctl_reqbufs" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0x6056a782, "v4l2_event_unsubscribe" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0xf9a482f9, "msleep" },
	{ 0x2659c370, "video_device_release_empty" },
	{ 0x2af59d26, "param_ops_int" },
	{ 0x7301854c, "v4l2_device_unregister" },
	{ 0xc40c9cea, "v4l2_ctrl_handler_free" },
	{ 0xe042a9a1, "v4l2_ctrl_g_ctrl" },
	{ 0xc37e04c1, "vb2_fop_poll" },
	{ 0xeee018b0, "vb2_ioctl_streamon" },
	{ 0xb43f9365, "ktime_get" },
	{ 0xa693e4b3, "usb_kill_urb" },
	{ 0x7c98e5d4, "vb2_ops_wait_prepare" },
	{ 0xf7cace70, "__video_register_device" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x130a1c7a, "v4l2_device_register" },
	{ 0xc7fe592, "vb2_fop_read" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xf9c0b663, "strlcat" },
	{ 0x710b9f3, "v4l2_device_disconnect" },
	{ 0xfb578fc5, "memset" },
	{ 0x4a237a4e, "vb2_vmalloc_memops" },
	{ 0xab80f12b, "vb2_fop_mmap" },
	{ 0xe7947977, "vb2_ioctl_qbuf" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0x7c32d0f0, "printk" },
	{ 0x4f9d479a, "video_unregister_device" },
	{ 0xaed47d9d, "usb_set_interface" },
	{ 0xbee25b48, "v4l2_ctrl_subscribe_event" },
	{ 0xa90808df, "vb2_plane_vaddr" },
	{ 0xe28166ef, "vb2_buffer_done" },
	{ 0x5792f848, "strlcpy" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0x229a93ef, "vb2_ioctl_create_bufs" },
	{ 0x44b21b40, "usb_free_coherent" },
	{ 0x3071d9ad, "vb2_ioctl_dqbuf" },
	{ 0x40571b16, "__v4l2_ctrl_s_ctrl" },
	{ 0x9d49ca67, "usb_submit_urb" },
	{ 0x43060836, "vb2_fop_release" },
	{ 0xad85ceef, "vb2_queue_error" },
	{ 0x3124ce59, "video_devdata" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x427bc604, "input_register_device" },
	{ 0xdca4e2a, "v4l2_ctrl_handler_setup" },
	{ 0x7e72a9d8, "usb_clear_halt" },
	{ 0xaab455b7, "input_free_device" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0xbd38075a, "v4l2_fh_open" },
	{ 0x6dc57588, "vb2_ioctl_querybuf" },
	{ 0x37a0cba, "kfree" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x65c9b1c7, "input_unregister_device" },
	{ 0x55a7a91f, "vb2_ops_wait_finish" },
	{ 0xb0190b33, "usb_ifnum_to_if" },
	{ 0x28318305, "snprintf" },
	{ 0x615fc449, "vb2_ioctl_expbuf" },
	{ 0x986adb1, "usb_alloc_coherent" },
	{ 0xcdeedd3, "vb2_ioctl_streamoff" },
	{ 0x66fc7115, "v4l2_device_put" },
	{ 0xda9dc303, "usb_free_urb" },
	{ 0x41e9d366, "video_ioctl2" },
	{ 0xdde76930, "usb_alloc_urb" },
	{ 0xfffe7f76, "input_allocate_device" },
	{ 0xfedc8df2, "vb2_queue_init" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "D6AC426599C1C79BAAFB570");
