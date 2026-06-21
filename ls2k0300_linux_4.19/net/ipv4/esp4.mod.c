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
	{ 0xd2b09ce5, "__kmalloc" },
	{ 0x59e4b36a, "loongson_map_base" },
	{ 0x50b03c47, "sg_init_table" },
	{ 0xf01020a9, "skb_to_sgvec" },
	{ 0x58e48cb6, "xfrm_unregister_type" },
	{ 0xa466250f, "ipv4_update_pmtu" },
	{ 0x3b9e213a, "xfrm4_protocol_deregister" },
	{ 0xc4ed5445, "sg_next" },
	{ 0xec11e5dc, "pskb_put" },
	{ 0x5df2a77c, "skb_page_frag_refill" },
	{ 0x4f45d203, "skb_cow_data" },
	{ 0xe8ee46da, "__pskb_pull_tail" },
	{ 0x307cb8dc, "xfrm_input_resume" },
	{ 0xfb578fc5, "memset" },
	{ 0xe96ba557, "skb_checksum" },
	{ 0x7c32d0f0, "printk" },
	{ 0x3c3fce39, "__local_bh_enable_ip" },
	{ 0x98de77db, "xfrm_input" },
	{ 0x8b8890f1, "skb_push" },
	{ 0x4a0322fa, "cpu_data" },
	{ 0x37a02412, "xfrm_aalg_get_byname" },
	{ 0x9106c583, "crypto_aead_setkey" },
	{ 0x8e1af631, "__secpath_destroy" },
	{ 0xf1b12885, "xfrm_register_type" },
	{ 0x9c1e5bf5, "queued_spin_lock_slowpath" },
	{ 0xeca27aeb, "kfree_skb" },
	{ 0x31a9048f, "ipv4_redirect" },
	{ 0x6c240284, "crypto_destroy_tfm" },
	{ 0x8ca4c75d, "xfrm_output_resume" },
	{ 0x1f2d2dee, "skb_pull_rcsum" },
	{ 0x9f373e2e, "__xfrm_state_destroy" },
	{ 0xf4549c45, "km_new_mapping" },
	{ 0xe78446c5, "xfrm4_rcv" },
	{ 0xb5b87872, "xfrm4_protocol_register" },
	{ 0x42d4810e, "crypto_aead_setauthsize" },
	{ 0xe8339df0, "crypto_alloc_aead" },
	{ 0x5596d6a1, "xfrm_state_lookup" },
	{ 0x37a0cba, "kfree" },
	{ 0x4fcf188b, "memcpy" },
	{ 0x70d3aee8, "___pskb_trim" },
	{ 0x28318305, "snprintf" },
	{ 0x5f139fe4, "skb_copy_bits" },
	{ 0x235a7fff, "__put_page" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

