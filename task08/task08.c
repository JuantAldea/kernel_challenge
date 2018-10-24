#include <asm/mmu_context.h>
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>
#include <linux/rwsem.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Juan Antonio Aldea Armenteros");
MODULE_DESCRIPTION("Task 08 - debugfs");

static const char EUDYP_ID[] = "0123456789AB";
static const size_t EUDYP_ID_LEN = 12;

struct dentry *dir;

struct rw_semaphore rw_sem;
static char data[PAGE_SIZE];

static ssize_t id_driver_write(struct file *file, const char __user *buf,
	size_t count, loff_t *ppos);

static ssize_t id_driver_read(struct file *file, char __user *buf,
	size_t count, loff_t *ppos);

static const struct file_operations id_driver_fops = {
	.owner	= THIS_MODULE,
	.read	= id_driver_read,
	.write	= id_driver_write,
};

static ssize_t foo_driver_write(struct file *file, const char __user *buf,
	size_t len, loff_t *ppos);

static ssize_t foo_driver_read(struct file *file, char __user *buf,
	size_t len, loff_t *ppos);

static const struct file_operations foo_driver_fops = {
	.owner	= THIS_MODULE,
	.read	= foo_driver_read,
	.write	= foo_driver_write,
};

static ssize_t id_driver_write(struct file *file, const char __user *buf,
	size_t count, loff_t *ppos)
{
	char input[EUDYP_ID_LEN];
	ssize_t result = -EINVAL;

	if (count != EUDYP_ID_LEN) {
		pr_alert("BAD LEN0. %ld, %ld\n", count, EUDYP_ID_LEN);
		pr_alert("|%.20s|\n", buf);
		return -EINVAL;
	}

	if (count != strnlen_user(buf, EUDYP_ID_LEN + 2) - 1) {
		pr_alert("BAD LEN1. %ld, %ld\n", count, EUDYP_ID_LEN);
		pr_alert("|%.14s|", buf);
		return -EINVAL;
	}

	result = simple_write_to_buffer(input, EUDYP_ID_LEN, ppos, buf,
		EUDYP_ID_LEN);

	if (result < 0) {
		return result;
	}

	if (strncmp(input, EUDYP_ID, EUDYP_ID_LEN) == 0) {
		pr_alert("ALL OK.\n");
		return EUDYP_ID_LEN;
	}

	pr_alert("|%.14s|", buf);

	pr_alert("BAD STRING.\n");
	return -EINVAL;
}

static ssize_t id_driver_read(struct file *file, char __user *buf,
	size_t count, loff_t *ppos)
{
	return simple_read_from_buffer(buf, count, ppos, &EUDYP_ID,
		EUDYP_ID_LEN);
}

static ssize_t foo_driver_write(struct file *file, const char __user *buf,
	size_t len, loff_t *ppos)
{
	down_write(&rw_sem);

	size_t result = simple_write_to_buffer(data, PAGE_SIZE, ppos, buf, len);

	up_write(&rw_sem);
	return result;
}

static ssize_t foo_driver_read(struct file *file, char __user *buf,
	size_t len, loff_t *ppos)
{
	down_read(&rw_sem);

	size_t result = simple_read_from_buffer(buf, len, ppos, data,
		PAGE_SIZE);

	up_read(&rw_sem);
	return result;
}

static int __init init_m(void)
{
	dir = debugfs_create_dir("eudyptula", NULL);
	if (!dir) {
		pr_alert("debugfs eudyptula task 8: failed to create /sys/kernel/debug/eudyptula\n");
		return -ENODEV;
	}

	if (!debugfs_create_file("id", 0666, dir, NULL, &id_driver_fops)) {
		pr_alert("debugfs eudyptula task 8: failed to create /sys/kernel/debug/eudyptula/id\n");
		goto fail;
	}

	if (!debugfs_create_u32("jiffies", 0444, dir, (u32 *) &jiffies)) {
		pr_alert("debugfs eudyptula task 8: failed to create /sys/kernel/debug/eudyptula/jiffies\n");
		goto fail;
	}

	init_rwsem(&rw_sem);
	
	if (!debugfs_create_file_size("foo", 0644, dir, data, &foo_driver_fops,
		PAGE_SIZE)) {
		pr_alert("debugfs eudyptula task 8: failed to create /sys/kernel/debug/eudyptula/foo\n");
		goto fail;
	}

	pr_alert("MODULE LOADED.\n");

	return 0;

fail:
	debugfs_remove_recursive(dir);
	return -ENODEV;
}

static void __exit exit_m(void)
{
	debugfs_remove_recursive(dir);
	pr_alert("MODULE UNLOADED.\n");
}

module_init(init_m);
module_exit(exit_m);
