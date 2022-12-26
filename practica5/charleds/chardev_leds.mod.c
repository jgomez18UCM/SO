#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xc4162456, "module_layout" },
	{ 0x4a165127, "kobject_put" },
	{ 0xf99f641, "cdev_add" },
	{ 0x1bd8b5c, "cdev_init" },
	{ 0x94d30158, "cdev_alloc" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xeb50e36b, "cdev_del" },
	{ 0x92997ed8, "_printk" },
	{ 0x55385e2e, "__x86_indirect_thunk_r14" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0x754d539c, "strlen" },
	{ 0x87a21cb3, "__ubsan_handle_out_of_bounds" },
	{ 0x1a79c8e9, "__x86_indirect_thunk_r13" },
	{ 0x277dcb4, "vc_cons" },
	{ 0x4e6e8ea7, "fg_console" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "47F0A2595E2715FB6D24A22");
