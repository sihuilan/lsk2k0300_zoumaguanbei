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
	{ 0x2af59d26, "param_ops_int" },
	{ 0xd9a5ea54, "__init_waitqueue_head" },
	{ 0x1124d4ac, "proc_create_data" },
	{ 0xa0b46955, "proc_mkdir" },
	{ 0x85ad89d7, "tty_register_driver" },
	{ 0x3f68d86, "tty_port_link_device" },
	{ 0xf8fabe04, "tty_port_init" },
	{ 0x4113eda2, "tty_set_operations" },
	{ 0x67b27ec1, "tty_std_termios" },
	{ 0x60cd2a2c, "__tty_alloc_driver" },
	{ 0x8cb5f5bd, "put_tty_driver" },
	{ 0xf9654f70, "tty_unregister_driver" },
	{ 0x95ebac1f, "remove_proc_entry" },
	{ 0x91715312, "sprintf" },
	{ 0xce2962f3, "tty_check_change" },
	{ 0x7bee5376, "tty_wait_until_sent" },
	{ 0x1a7c0733, "tty_hung_up_p" },
	{ 0x4302d0eb, "free_pages" },
	{ 0x93fca811, "__get_free_pages" },
	{ 0x37110088, "remove_wait_queue" },
	{ 0x1000e51, "schedule" },
	{ 0x4afb2238, "add_wait_queue" },
	{ 0xaad8c7d6, "default_wake_function" },
	{ 0xeef40ea2, "do_SAK" },
	{ 0xf553db1b, "__tty_insert_flip_char" },
	{ 0x33029547, "tty_hangup" },
	{ 0x92540fbf, "finish_wait" },
	{ 0x8c26d495, "prepare_to_wait_event" },
	{ 0xfe487975, "init_wait_entry" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x8ddd8aad, "schedule_timeout" },
	{ 0x526c3a6c, "jiffies" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0x2e0d2f7f, "queue_work_on" },
	{ 0x2d3385d3, "system_wq" },
	{ 0x7b8ab8d2, "tty_insert_flip_string_fixed_flag" },
	{ 0x4e99dbda, "tty_buffer_request_room" },
	{ 0x37a0cba, "kfree" },
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0xe6fd8086, "tty_flip_buffer_push" },
	{ 0xe526f136, "__copy_user" },
	{ 0xfb578fc5, "memset" },
	{ 0x3eeb2322, "__wake_up" },
	{ 0xbb763f8d, "tty_register_device" },
	{ 0xcf2a6966, "up" },
	{ 0x6626afca, "down" },
	{ 0x6330fcab, "PDE_DATA" },
	{ 0xc6cbbc89, "capable" },
	{ 0xa1e746d3, "try_module_get" },
	{ 0x4322b27b, "module_put" },
	{ 0x4a124140, "tty_unregister_device" },
	{ 0x7c32d0f0, "printk" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "FD853119E597DF8871AAF1F");
