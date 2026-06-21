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
	{ 0xf6d47e86, "snd_hda_gen_free" },
	{ 0x51a707e, "snd_hda_codec_get_pin_target" },
	{ 0xb3021ed6, "snd_hda_mixer_amp_volume_put" },
	{ 0xfd472db9, "snd_hdac_codec_read" },
	{ 0xbea62d9, "snd_hda_gen_parse_auto_config" },
	{ 0x8e3fb99a, "snd_hda_apply_fixup" },
	{ 0xdceec10e, "snd_hda_add_verbs" },
	{ 0x3052dbc8, "snd_hda_pick_fixup" },
	{ 0xe2f4dd30, "snd_hda_mixer_amp_tlv" },
	{ 0xa6093a32, "mutex_unlock" },
	{ 0xd6aff556, "snd_hda_gen_build_pcms" },
	{ 0x676f5ac, "snd_hda_get_input_pin_attr" },
	{ 0xf5531b7b, "snd_hda_override_conn_list" },
	{ 0x9dc09a12, "snd_hda_jack_unsol_event" },
	{ 0xe9d9ebd5, "snd_hda_codec_set_pincfg" },
	{ 0x4b34f72, "snd_hda_mixer_amp_switch_put" },
	{ 0xcbc67479, "snd_hda_gen_update_outputs" },
	{ 0xe2d5255a, "strcmp" },
	{ 0x1cc992a, "snd_hda_jack_detect_enable_callback" },
	{ 0x63df65bd, "input_event" },
	{ 0x9e3fd6fc, "_dev_warn" },
	{ 0x5572352e, "snd_hda_mixer_amp_volume_get" },
	{ 0x58a3db91, "snd_hda_codec_set_name" },
	{ 0x4582da0f, "snd_hdac_regmap_write_raw" },
	{ 0xd72fe776, "_snd_hda_set_pin_ctl" },
	{ 0x628fec61, "snd_hda_gen_hp_automute" },
	{ 0xe0d90f2f, "snd_hda_gen_check_power_status" },
	{ 0x7c32d0f0, "printk" },
	{ 0x20c55ae0, "sscanf" },
	{ 0xa6647214, "snd_hda_codec_set_pin_target" },
	{ 0x6add5c9a, "dmi_find_device" },
	{ 0xe2c3648a, "snd_hda_mixer_amp_switch_get" },
	{ 0xe8d0da47, "snd_hda_gen_add_micmute_led" },
	{ 0x530c2bb4, "snd_hdac_power_down_pm" },
	{ 0x41aed6e7, "mutex_lock" },
	{ 0x9026b565, "snd_hda_gen_path_power_filter" },
	{ 0x119a1c0b, "snd_hda_gen_init" },
	{ 0x756576cd, "snd_hda_parse_pin_defcfg" },
	{ 0x8bac7c25, "snd_hda_jack_set_gating_jack" },
	{ 0xf6ff132f, "__hda_codec_driver_register" },
	{ 0x33dce5f6, "_dev_err" },
	{ 0x5adae254, "snd_hda_override_amp_caps" },
	{ 0x5441cce6, "snd_hda_jack_detect_state" },
	{ 0xb4f19fc3, "snd_hda_find_mixer_ctl" },
	{ 0xc6525cf8, "snd_hda_sequence_write" },
	{ 0x35b5cd3, "snd_hda_get_bool_hint" },
	{ 0x2f9bd219, "_dev_info" },
	{ 0x2816ae4c, "snd_hdac_override_parm" },
	{ 0x615a1e18, "snd_hda_pick_pin_fixup" },
	{ 0x427bc604, "input_register_device" },
	{ 0x2f94440c, "_snd_hdac_read_parm" },
	{ 0xaab455b7, "input_free_device" },
	{ 0xd136ce79, "snd_hda_gen_reboot_notify" },
	{ 0xbc877a13, "kmem_cache_alloc_trace" },
	{ 0x58072fea, "snd_hda_gen_add_kctl" },
	{ 0x37a0cba, "kfree" },
	{ 0x65c9b1c7, "input_unregister_device" },
	{ 0xea8311d4, "snd_hdac_codec_write" },
	{ 0x228b08b0, "snd_hda_codec_get_pincfg" },
	{ 0xfb18d078, "snd_hda_apply_pincfgs" },
	{ 0x3e7c197c, "hda_codec_driver_unregister" },
	{ 0xef6f1003, "snd_hda_mixer_amp_switch_info" },
	{ 0x2578c6f, "snd_hda_gen_spec_init" },
	{ 0x58de41a7, "snd_hda_mixer_amp_volume_info" },
	{ 0x2410e644, "query_amp_caps" },
	{ 0x62c74ae, "regcache_sync" },
	{ 0xb6ec43c7, "snd_hda_gen_build_controls" },
	{ 0xd89e1b9a, "snd_hdac_power_up_pm" },
	{ 0xe914e41e, "strcpy" },
	{ 0xc16a0a6b, "snd_hda_apply_verbs" },
	{ 0xfffe7f76, "input_allocate_device" },
	{ 0x17787154, "snd_hda_shutup_pins" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=snd-hda-codec-generic,snd-hda-codec,snd-hda-core";

MODULE_ALIAS("hdaudio:v10EC0215r*a01*");
MODULE_ALIAS("hdaudio:v10EC0221r*a01*");
MODULE_ALIAS("hdaudio:v10EC0222r*a01*");
MODULE_ALIAS("hdaudio:v10EC0225r*a01*");
MODULE_ALIAS("hdaudio:v10EC0231r*a01*");
MODULE_ALIAS("hdaudio:v10EC0233r*a01*");
MODULE_ALIAS("hdaudio:v10EC0234r*a01*");
MODULE_ALIAS("hdaudio:v10EC0235r*a01*");
MODULE_ALIAS("hdaudio:v10EC0236r*a01*");
MODULE_ALIAS("hdaudio:v10EC0245r*a01*");
MODULE_ALIAS("hdaudio:v10EC0255r*a01*");
MODULE_ALIAS("hdaudio:v10EC0256r*a01*");
MODULE_ALIAS("hdaudio:v10EC0257r*a01*");
MODULE_ALIAS("hdaudio:v10EC0260r*a01*");
MODULE_ALIAS("hdaudio:v10EC0262r*a01*");
MODULE_ALIAS("hdaudio:v10EC0267r*a01*");
MODULE_ALIAS("hdaudio:v10EC0268r*a01*");
MODULE_ALIAS("hdaudio:v10EC0269r*a01*");
MODULE_ALIAS("hdaudio:v10EC0270r*a01*");
MODULE_ALIAS("hdaudio:v10EC0272r*a01*");
MODULE_ALIAS("hdaudio:v10EC0274r*a01*");
MODULE_ALIAS("hdaudio:v10EC0275r*a01*");
MODULE_ALIAS("hdaudio:v10EC0276r*a01*");
MODULE_ALIAS("hdaudio:v10EC0280r*a01*");
MODULE_ALIAS("hdaudio:v10EC0282r*a01*");
MODULE_ALIAS("hdaudio:v10EC0283r*a01*");
MODULE_ALIAS("hdaudio:v10EC0284r*a01*");
MODULE_ALIAS("hdaudio:v10EC0285r*a01*");
MODULE_ALIAS("hdaudio:v10EC0286r*a01*");
MODULE_ALIAS("hdaudio:v10EC0287r*a01*");
MODULE_ALIAS("hdaudio:v10EC0288r*a01*");
MODULE_ALIAS("hdaudio:v10EC0289r*a01*");
MODULE_ALIAS("hdaudio:v10EC0290r*a01*");
MODULE_ALIAS("hdaudio:v10EC0292r*a01*");
MODULE_ALIAS("hdaudio:v10EC0293r*a01*");
MODULE_ALIAS("hdaudio:v10EC0294r*a01*");
MODULE_ALIAS("hdaudio:v10EC0295r*a01*");
MODULE_ALIAS("hdaudio:v10EC0298r*a01*");
MODULE_ALIAS("hdaudio:v10EC0299r*a01*");
MODULE_ALIAS("hdaudio:v10EC0300r*a01*");
MODULE_ALIAS("hdaudio:v10EC0623r*a01*");
MODULE_ALIAS("hdaudio:v10EC0861r00100340a01*");
MODULE_ALIAS("hdaudio:v10EC0660r*a01*");
MODULE_ALIAS("hdaudio:v10EC0861r*a01*");
MODULE_ALIAS("hdaudio:v10EC0862r*a01*");
MODULE_ALIAS("hdaudio:v10EC0662r00100002a01*");
MODULE_ALIAS("hdaudio:v10EC0662r00100101a01*");
MODULE_ALIAS("hdaudio:v10EC0662r00100300a01*");
MODULE_ALIAS("hdaudio:v10EC0663r*a01*");
MODULE_ALIAS("hdaudio:v10EC0665r*a01*");
MODULE_ALIAS("hdaudio:v10EC0667r*a01*");
MODULE_ALIAS("hdaudio:v10EC0668r*a01*");
MODULE_ALIAS("hdaudio:v10EC0670r*a01*");
MODULE_ALIAS("hdaudio:v10EC0671r*a01*");
MODULE_ALIAS("hdaudio:v10EC0680r*a01*");
MODULE_ALIAS("hdaudio:v10EC0700r*a01*");
MODULE_ALIAS("hdaudio:v10EC0701r*a01*");
MODULE_ALIAS("hdaudio:v10EC0703r*a01*");
MODULE_ALIAS("hdaudio:v10EC0711r*a01*");
MODULE_ALIAS("hdaudio:v10EC0867r*a01*");
MODULE_ALIAS("hdaudio:v10EC0880r*a01*");
MODULE_ALIAS("hdaudio:v10EC0882r*a01*");
MODULE_ALIAS("hdaudio:v10EC0883r*a01*");
MODULE_ALIAS("hdaudio:v10EC0885r00100101a01*");
MODULE_ALIAS("hdaudio:v10EC0885r00100103a01*");
MODULE_ALIAS("hdaudio:v10EC0885r*a01*");
MODULE_ALIAS("hdaudio:v10EC0887r*a01*");
MODULE_ALIAS("hdaudio:v10EC0888r00100101a01*");
MODULE_ALIAS("hdaudio:v10EC0888r*a01*");
MODULE_ALIAS("hdaudio:v10EC0889r*a01*");
MODULE_ALIAS("hdaudio:v10EC0892r*a01*");
MODULE_ALIAS("hdaudio:v10EC0897r*a01*");
MODULE_ALIAS("hdaudio:v10EC0899r*a01*");
MODULE_ALIAS("hdaudio:v10EC0900r*a01*");
MODULE_ALIAS("hdaudio:v10EC0B00r*a01*");
MODULE_ALIAS("hdaudio:v10EC1168r*a01*");
MODULE_ALIAS("hdaudio:v10EC1220r*a01*");
