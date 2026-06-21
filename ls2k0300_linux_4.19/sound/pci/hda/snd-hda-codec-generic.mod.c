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
	{ 0xd9846d93, "kmalloc_caches" },
	{ 0xf9a482f9, "msleep" },
	{ 0x51a707e, "snd_hda_codec_get_pin_target" },
	{ 0x349cba85, "strchr" },
	{ 0x79ca405a, "snd_hda_multi_out_dig_prepare" },
	{ 0x375cacd5, "snd_hda_input_mux_info" },
	{ 0xb3021ed6, "snd_hda_mixer_amp_volume_put" },
	{ 0xfd472db9, "snd_hdac_codec_read" },
	{ 0x9a8e1877, "snd_array_free" },
	{ 0xc9e08b67, "snd_hda_get_default_vref" },
	{ 0x87eedefe, "snd_hda_enum_helper_info" },
	{ 0x8e3fb99a, "snd_hda_apply_fixup" },
	{ 0x8b515ee6, "snd_hda_sync_vmaster_hook" },
	{ 0x1c823978, "snd_hda_codec_amp_stereo" },
	{ 0xa338307b, "__snd_hda_add_vmaster" },
	{ 0xbe7dd7dc, "snd_array_new" },
	{ 0xe2f4dd30, "snd_hda_mixer_amp_tlv" },
	{ 0x6beb3b8c, "snd_hda_jack_add_kctls" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0x676f5ac, "snd_hda_get_input_pin_attr" },
	{ 0x31f89a41, "snd_hda_check_amp_list_power" },
	{ 0x9dc09a12, "snd_hda_jack_unsol_event" },
	{ 0x91715312, "sprintf" },
	{ 0xc499ae1e, "kstrdup" },
	{ 0xe118de5d, "snd_pcm_2_1_chmaps" },
	{ 0x4b34f72, "snd_hda_mixer_amp_switch_put" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x1cc992a, "snd_hda_jack_detect_enable_callback" },
	{ 0xf9c0b663, "strlcat" },
	{ 0x982df650, "snd_hda_check_amp_caps" },
	{ 0xfb578fc5, "memset" },
	{ 0x5572352e, "snd_hda_mixer_amp_volume_get" },
	{ 0x4582da0f, "snd_hdac_regmap_write_raw" },
	{ 0x11089ac7, "_ctype" },
	{ 0xd72fe776, "_snd_hda_set_pin_ctl" },
	{ 0x9a76f11f, "__mutex_init" },
	{ 0xa6647214, "snd_hda_codec_set_pin_target" },
	{ 0xe2c3648a, "snd_hda_mixer_amp_switch_get" },
	{ 0xafe3b62d, "snd_hda_codec_set_power_to_all" },
	{ 0xb58dc7c8, "snd_hda_get_conn_list" },
	{ 0x341de195, "snd_ctl_boolean_stereo_info" },
	{ 0xd42be326, "snd_hda_multi_out_analog_prepare" },
	{ 0xf5d7b648, "snd_hda_multi_out_analog_cleanup" },
	{ 0x5792f848, "strlcpy" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0xd5a743d9, "is_jack_detectable" },
	{ 0x4102a59d, "snd_hda_create_spdif_in_ctls" },
	{ 0xde4ec187, "snd_hda_codec_amp_init" },
	{ 0x756576cd, "snd_hda_parse_pin_defcfg" },
	{ 0xe3a53f4c, "sort" },
	{ 0xf6ff132f, "__hda_codec_driver_register" },
	{ 0x33dce5f6, "_dev_err" },
	{ 0x2dc72a1c, "snd_hda_get_int_hint" },
	{ 0x5441cce6, "snd_hda_jack_detect_state" },
	{ 0x420524a9, "snd_hda_jack_tbl_get" },
	{ 0x81b69e41, "snd_ctl_enum_info" },
	{ 0xb4f19fc3, "snd_hda_find_mixer_ctl" },
	{ 0xc17ce2c2, "snd_hda_create_spdif_share_sw" },
	{ 0x35b5cd3, "snd_hda_get_bool_hint" },
	{ 0x5666c732, "snd_hda_multi_out_analog_open" },
	{ 0xed7d20a0, "snd_hda_get_conn_index" },
	{ 0x9b25529c, "snd_hda_multi_out_dig_close" },
	{ 0x6122ff86, "snd_hda_create_dig_out_ctls" },
	{ 0x2f94440c, "_snd_hdac_read_parm" },
	{ 0xe0ad2f3c, "snd_hda_correct_pin_ctl" },
	{ 0x6bc8392c, "snd_hda_add_imux_item" },
	{ 0x2dba7b52, "snd_hda_get_pin_label" },
	{ 0x5dd59a2f, "snd_hda_multi_out_dig_cleanup" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x1d6f5588, "snd_ctl_sync_vmaster" },
	{ 0x6dfe45ef, "__snd_hda_codec_cleanup_stream" },
	{ 0x191923cd, "snd_hda_codec_pcm_new" },
	{ 0x37a0cba, "kfree" },
	{ 0x5d00e8c3, "snd_hda_add_vmaster_hook" },
	{ 0x4fcf188b, "memcpy" },
	{ 0xa37c2eb6, "snd_hda_multi_out_dig_open" },
	{ 0x4ca9669f, "scnprintf" },
	{ 0xea8311d4, "snd_hdac_codec_write" },
	{ 0x74c134b9, "__sw_hweight32" },
	{ 0x228b08b0, "snd_hda_codec_get_pincfg" },
	{ 0x3e7c197c, "hda_codec_driver_unregister" },
	{ 0xef6f1003, "snd_hda_mixer_amp_switch_info" },
	{ 0x491024e1, "snd_hda_codec_setup_stream" },
	{ 0x28318305, "snprintf" },
	{ 0xbe66560, "memmove" },
	{ 0x58de41a7, "snd_hda_mixer_amp_volume_info" },
	{ 0x2410e644, "query_amp_caps" },
	{ 0x90a7d487, "hda_get_autocfg_input_label" },
	{ 0x62c74ae, "regcache_sync" },
	{ 0xdddcad98, "snd_hda_set_vmaster_tlv" },
	{ 0xb93f0c5a, "snd_hda_codec_amp_init_stereo" },
	{ 0xb4adf2af, "snd_hda_get_connections" },
	{ 0x573e6cb4, "snd_hda_add_new_ctls" },
	{ 0xe914e41e, "strcpy" },
	{ 0x5dc0f0d1, "snd_hda_codec_amp_update" },
	{ 0xc16a0a6b, "snd_hda_apply_verbs" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=snd-hda-codec,snd-hda-core";

MODULE_ALIAS("hdaudio:v00000201r*a01*");
