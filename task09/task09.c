#include <asm/mmu_context.h>
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/jiffies.h>
#include <linux/rwsem.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Juan Antonio Aldea Armenteros");
MODULE_DESCRIPTION("Task 09 - sysfs");

static const char EUDYP_ID[] = "0123456789AB";
static const size_t EUDYP_ID_LEN = 12;

struct rw_semaphore rw_sem;
static char data[PAGE_SIZE];


static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	size_t result;
	int lock_state;

	lock_state = down_write_killable(&rw_sem);

	if (lock_state == -EINTR) {
		return  -EINTR;
	}

	result = scnprintf(data, count < PAGE_SIZE ? count + 1 : PAGE_SIZE, "%s", buf);

	up_write(&rw_sem);

	return result;
}

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	size_t result;
	int lock_state;

	lock_state = down_read_killable(&rw_sem);

	if (lock_state == -EINTR) {
		return -EINTR;
	}

	result = scnprintf(buf, PAGE_SIZE, "%s\n", data);


	up_read(&rw_sem);

	return result;
}

static struct kobj_attribute foo_attribute = __ATTR_RW(foo);


static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	char input[EUDYP_ID_LEN + 1];
	ssize_t result = -EINVAL;

	if (count != EUDYP_ID_LEN) {
		pr_alert("WRONG LEN0. %ld, %ld\n", count, EUDYP_ID_LEN);
		pr_alert("|%.20s|\n", buf);
		return -EINVAL;
	}

	result = scnprintf(input, sizeof(input), "%s", buf);

	if (result < 0) {
		return result;
	}

	if (strncmp(input, EUDYP_ID, EUDYP_ID_LEN) == 0) {
		pr_alert("ALL OK.\n");
		return EUDYP_ID_LEN;
	}

	pr_alert("WRONG STRING.\n");
	pr_alert("|%.14s|\n", buf);

	return -EINVAL;
}

static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	return scnprintf(buf, EUDYP_ID_LEN + 2, "%s\n", EUDYP_ID);
}

static struct kobj_attribute id_attribute = __ATTR_RW(id);


static ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	return scnprintf(buf, PAGE_SIZE, "%lu\n", jiffies);
}

static struct kobj_attribute jiffies_attribute = __ATTR_RO(jiffies);

static struct attribute *attrs[] = {
	&foo_attribute.attr,
	&id_attribute.attr,
	&jiffies_attribute.attr,
	NULL,
};

static struct attribute_group attr_group = {
	.attrs = attrs,
};

static struct kobject *example_kobj;

static int __init init_m(void)
{
	int retval;

	init_rwsem(&rw_sem);

	example_kobj = kobject_create_and_add("eudyptula", kernel_kobj);

	if (!example_kobj) {
		return -ENOMEM;
	}

	retval = sysfs_create_group(example_kobj, &attr_group);

	if (retval) {
		kobject_put(example_kobj);
	}

	return retval;
}

static void __exit exit_m(void)
{
	kobject_put(example_kobj);
}

module_init(init_m);
module_exit(exit_m);
