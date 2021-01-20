#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/kthread.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Juan Antonio Aldea Armenteros");
MODULE_DESCRIPTION("Task 18.");

static int id_counter;
static DEFINE_MUTEX(identity_list_mutex);
static DECLARE_WAIT_QUEUE_HEAD(wee_wait);

static LIST_HEAD(identity_list);

static const char DEVICE_NAME[] = "eudyptula";

static struct task_struct *eudyptula_kthread;

#define NAME_LENGTH 20

struct identity {
	char  name[NAME_LENGTH];
	int   id;
	bool  busy;
	struct list_head list;
};


static struct identity *identity_find(int id);
static int identity_create(char *name, int id);
static void identity_destroy(void);
struct identity *identity_get(void);

struct identity *identity_get(void)
{
	if (mutex_lock_interruptible(&identity_list_mutex) == -EINTR)
		return NULL;

	struct identity *ptr = list_first_entry_or_null(&identity_list,
		struct identity, list);

	if (ptr)
		list_del(&ptr->list);

	mutex_unlock(&identity_list_mutex);

	return ptr;
}

static int identity_create(char *name, int id)
{
	if (mutex_lock_interruptible(&identity_list_mutex) == -EINTR)
		return -EINTR;

	int ret = 0;

	if (identity_find(id)) {
		pr_alert("ID %d not added: already exists\n", id);
		ret = -EINVAL;
		goto end;
	}

	struct identity *node = kmalloc(sizeof(*node), GFP_KERNEL);

	if (!node) {
		ret = -ENOMEM;
		goto end;
	}

	strncpy(node->name, name, NAME_LENGTH);
	node->name[NAME_LENGTH - 1] = '\0';
	node->id = id;
	node->busy = false;

	list_add_tail(&node->list, &identity_list);
	pr_alert("Added %s %d %d", node->name, node->id, node->busy);

end:
	mutex_unlock(&identity_list_mutex);
	return ret;
}

static struct identity *identity_find(int id)
{
	struct identity *data_ptr;

	list_for_each_entry(data_ptr, &identity_list, list) {
		if (id == data_ptr->id)
			return data_ptr;
	}
	return NULL;
}

static void identity_destroy(void)
{
	struct identity *cursor, *next;

	list_for_each_entry_safe(cursor, next, &identity_list, list) {
		list_del(&cursor->list);
		kfree(cursor);
	}
}

static ssize_t misc_driver_write(struct file *file, const char __user *buf,
	size_t len, loff_t *ppos);

static const struct file_operations misc_driver_fops = {
	.owner	= THIS_MODULE,
	.write	= misc_driver_write,
};

static struct miscdevice misc_driver_dev = {
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= DEVICE_NAME,
	.fops	= &misc_driver_fops,
	.mode   = S_IWUGO,
};

static ssize_t misc_driver_write(struct file *file, const char __user *buf,
	size_t count, loff_t *ppos)
{
	char input[NAME_LENGTH] = {0};
	ssize_t result = -EINVAL;
	const size_t len = count < NAME_LENGTH ? count : NAME_LENGTH - 1;

	result = simple_write_to_buffer(input, NAME_LENGTH - 1, ppos, buf, len);

	if (result < 0)
		return result;

	if (identity_create(input, id_counter++))
		return -EINVAL;

	wake_up(&wee_wait);
	return count;
}

static int kthread_fn(void *unused)
{
	while (!kthread_should_stop()) {
		wait_event(wee_wait,
			kthread_should_stop() || !list_empty(&identity_list));
		struct identity *ptr = identity_get();

		if (ptr) {
			msleep_interruptible(5000);
			pr_alert("rm ID: %d; Name: %s.", ptr->id, ptr->name);
			kfree(ptr);
		}
	}

	pr_alert("kthread exiting.\n");
	return 0;
}

static int __init init_m(void)
{
	id_counter = 0;
	eudyptula_kthread = kthread_run(kthread_fn, NULL, "eudyptula");

	if (eudyptula_kthread == ERR_PTR(-ENOMEM))
		return -ENOMEM;

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
	kthread_stop(eudyptula_kthread);
	wake_up(&wee_wait);
	identity_destroy();
	misc_deregister(&misc_driver_dev);
	pr_alert("MODULE UNLOADED.\n");
}

module_init(init_m);
module_exit(exit_m);
