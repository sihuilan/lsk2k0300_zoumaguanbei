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
	{ 0xe56a9336, "snd_pcm_format_width" },
	{ 0xaef47267, "kobject_put" },
	{ 0x8487a2b6, "flush_work" },
	{ 0x2d3385d3, "system_wq" },
	{ 0xac0d64e5, "bus_register" },
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0xf9a482f9, "msleep" },
	{ 0x862258db, "timecounter_init" },
	{ 0x2c422fe4, "trace_handle_return" },
	{ 0xc6844b89, "regmap_exit" },
	{ 0xee8f3c66, "__pm_runtime_suspend" },
	{ 0xfed4da3e, "kobject_uevent" },
	{ 0xad27f361, "__warn_printk" },
	{ 0x88bfa7e, "cancel_work_sync" },
	{ 0x448eac3e, "kmemdup" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x53adbfb8, "__pm_runtime_resume" },
	{ 0x82c3f1bd, "trace_event_buffer_reserve" },
	{ 0x945656e7, "regmap_read" },
	{ 0x91715312, "sprintf" },
	{ 0x851ff248, "bpf_trace_run3" },
	{ 0x1fc9dc2c, "sysfs_remove_group" },
	{ 0xc499ae1e, "kstrdup" },
	{ 0x526c3a6c, "jiffies" },
	{ 0xc9fb4696, "kobject_create_and_add" },
	{ 0x44b5ee9a, "kasprintf" },
	{ 0xe526f136, "__copy_user" },
	{ 0xe9cfe151, "trace_define_field" },
	{ 0x9e3fd6fc, "_dev_warn" },
	{ 0xfb578fc5, "memset" },
	{ 0x3b4c918a, "device_del" },
	{ 0xcfdca50, "__regmap_init" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0x7c32d0f0, "printk" },
	{ 0xa9cd7b5d, "sysfs_create_group" },
	{ 0x1c3e02e4, "memcmp" },
	{ 0x3d9f21fe, "__cpu_online_mask" },
	{ 0x9ec6ca96, "ktime_get_real_ts64" },
	{ 0x1edb69d6, "ktime_get_raw_ts64" },
	{ 0x3e7b0367, "pm_runtime_get_if_in_use" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x577c8031, "trace_event_reg" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0x373db350, "kstrtoint" },
	{ 0xb13ec247, "device_add" },
	{ 0x8244c66d, "kobject_add" },
	{ 0x33dce5f6, "_dev_err" },
	{ 0xcd401818, "bus_unregister" },
	{ 0x1e8bbdc5, "perf_trace_run_bpf_submit" },
	{ 0x7266d6f3, "perf_trace_buf_alloc" },
	{ 0x9f46ced8, "__sw_hweight64" },
	{ 0x12a38747, "usleep_range" },
	{ 0x511b4c1e, "trace_event_ignore_this_pid" },
	{ 0xa61db944, "put_device" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x1d24c881, "___ratelimit" },
	{ 0x4b1a6fdb, "__pm_runtime_set_status" },
	{ 0xd3eb029a, "trace_event_buffer_commit" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x9f7d3241, "event_triggers_call" },
	{ 0xd0bb574b, "bpf_trace_run2" },
	{ 0x37a0cba, "kfree" },
	{ 0x4fcf188b, "memcpy" },
	{ 0xc7bc73a8, "trace_event_raw_init" },
	{ 0x46041cc4, "device_initialize" },
	{ 0x5b436d4a, "kobject_init" },
	{ 0x4ca9669f, "scnprintf" },
	{ 0x26e665ed, "snd_pcm_add_chmap_ctls" },
	{ 0x2f891044, "trace_raw_output_prep" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0x28318305, "snprintf" },
	{ 0xc6e06ce3, "trace_seq_printf" },
	{ 0x6fed1cec, "dev_set_name" },
	{ 0x5e515be6, "ktime_get_ts64" },
	{ 0x9dc49a80, "regmap_write" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0x85f5e2aa, "krealloc" },
	{ 0x6c07d933, "add_uevent_var" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

