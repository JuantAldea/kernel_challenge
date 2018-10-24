#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Juan Antonio Aldea Armenteros");
MODULE_DESCRIPTION("Prints hello world into the kernel log as debug message.");

static const char EUDYP_ID[] = "0123456789AB";
static const size_t EUDYP_ID_LEN = 12;
static const char DEVICE_NAME[] = "eudyptula";

static ssize_t misc_driver_write(struct file *file, const char __user *buf,
	size_t len, loff_t *ppos);

static ssize_t misc_driver_read(struct file *file, char __user *buf,
	size_t len, loff_t *ppos);

static const struct file_operations misc_driver_fops = {
	.owner	= THIS_MODULE,
	.read	= misc_driver_read,
	.write	= misc_driver_write,
};

static struct miscdevice misc_driver_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVICE_NAME,
	.fops	= &misc_driver_fops,
};

static ssize_t misc_driver_write(struct file *file, const char __user *buf,
	size_t count, loff_t *ppos)
{
	char input[EUDYP_ID_LEN];
	ssize_t result = -EINVAL;

	if (count != EUDYP_ID_LEN) {
		pr_alert("BAD LEN0.\n");
		return -EINVAL;
	}

	if (count != strnlen_user(buf, EUDYP_ID_LEN + 2) - 1) {
		pr_alert("BAD LEN1.\n");
		return -EINVAL;
	}

	result = simple_write_to_buffer(input, EUDYP_ID_LEN, ppos, buf, EUDYP_ID_LEN);

	if (result < 0) {
		return result;
	}

	if (strncmp(input, EUDYP_ID, EUDYP_ID_LEN) == 0) {
		pr_alert("ALL OK.\n");
		return EUDYP_ID_LEN;
	}

	pr_alert("BAD STRING.\n");
	return -EINVAL;
}

static ssize_t misc_driver_read(struct file *file, char __user *buf,
	size_t count, loff_t *ppos)
{
	return simple_read_from_buffer(buf, count, ppos, &EUDYP_ID, EUDYP_ID_LEN);
}

static int __init init_m(void)
{
	int result = misc_register(&misc_driver_dev);

	if (result) {
		pr_alert("Error while registering the device.\n");
		return result;
	}

	pr_alert("MODULE LOADED.\n");
	return result;
}

static void __exit exit_m(void)
{
	misc_deregister(&misc_driver_dev);
	pr_alert("MODULE UNLOADED.\n");
}

module_init(init_m);
module_exit(exit_m);

