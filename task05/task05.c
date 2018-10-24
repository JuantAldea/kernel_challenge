#include <linux/module.h>
#include <linux/hid.h>
#include <linux/usb/input.h>

static void __exit usb_exit(void);
static int  __init usb_init(void);

static const  struct usb_device_id hello_id_table[] = {
	{
		USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			USB_INTERFACE_SUBCLASS_BOOT,
			USB_INTERFACE_PROTOCOL_KEYBOARD
		)
	},
	{}
};

static void __exit usb_exit(void)
{
	pr_alert("Task05 module removed\n");
}

static int __init usb_init(void)
{
	pr_alert("Task05 module INIT\n");
	return 0;
}

module_exit(usb_exit);
module_init(usb_init);
MODULE_DEVICE_TABLE(usb, hello_id_table);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Juan Antonio Aldea Armenteros");
MODULE_DESCRIPTION("Eudyptula Challenge - Task 05");

