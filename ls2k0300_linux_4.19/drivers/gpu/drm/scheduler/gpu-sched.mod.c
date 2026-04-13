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
	{ 0x9c38cd9f, "kmem_cache_destroy" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x2c422fe4, "trace_handle_return" },
	{ 0xe123f3d9, "dma_fence_release" },
	{ 0x60a13e90, "rcu_barrier" },
	{ 0xffeedf6a, "delayed_work_timer_fn" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0xf6c1256d, "cancel_delayed_work_sync" },
	{ 0x82c3f1bd, "trace_event_buffer_reserve" },
	{ 0xdda39e5a, "kthread_create_on_node" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0xe9cfe151, "trace_define_field" },
	{ 0xfb578fc5, "memset" },
	{ 0xdb234a74, "bpf_trace_run1" },
	{ 0xc5e9aa1f, "dma_fence_init" },
	{ 0x7c32d0f0, "printk" },
	{ 0x49e2c35b, "kthread_stop" },
	{ 0x3d9f21fe, "__cpu_online_mask" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x577c8031, "trace_event_reg" },
	{ 0xe5a21497, "kmem_cache_free" },
	{ 0xb0fa0d15, "dma_fence_signal" },
	{ 0x14605535, "dma_fence_context_alloc" },
	{ 0x1e8bbdc5, "perf_trace_run_bpf_submit" },
	{ 0x7266d6f3, "perf_trace_buf_alloc" },
	{ 0x1dd71f7a, "drm_err" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0x7773821f, "kthread_unpark" },
	{ 0x57a0c51d, "kmem_cache_alloc" },
	{ 0x45f35102, "dma_fence_remove_callback" },
	{ 0x511b4c1e, "trace_event_ignore_this_pid" },
	{ 0x164e7c53, "queue_delayed_work_on" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x1000e51, "schedule" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0xd3eb029a, "trace_event_buffer_commit" },
	{ 0xbb313e3e, "wake_up_process" },
	{ 0x79defbe1, "kthread_should_park" },
	{ 0xbe59bc38, "kthread_park" },
	{ 0xfece5f82, "kmem_cache_create" },
	{ 0x93511cc0, "sched_setscheduler" },
	{ 0x8ee32c83, "dma_fence_add_callback" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0x9f7d3241, "event_triggers_call" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xd0bb574b, "bpf_trace_run2" },
	{ 0xf95322f4, "kthread_parkme" },
	{ 0x37a0cba, "kfree" },
	{ 0x4fcf188b, "memcpy" },
	{ 0xc7bc73a8, "trace_event_raw_init" },
	{ 0x643e0ce5, "call_rcu_sched" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x2f891044, "trace_raw_output_prep" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0xc6e06ce3, "trace_seq_printf" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

