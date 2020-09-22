#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>

#define NAME_LENGTH 20

struct identity {
	char  name[NAME_LENGTH];
	int   id;
	bool  busy;
	struct list_head list;
};

static LIST_HEAD(identity_list);

static struct identity *identity_find(int id);
static int identity_create(char *name, int id);
static void identity_destroy(int id);

static int identity_create(char *name, int id)
{
	if (identity_find(id)) {
		pr_debug("ID %d not added: already exists\n", id);
		return -EINVAL;
	}

	struct identity *node = kmalloc(sizeof(*node), GFP_KERNEL);

	if (!node)
		return -ENOMEM;

	strncpy(node->name, name, NAME_LENGTH);
	node->name[NAME_LENGTH - 1] = '\0';
	node->id = id;
	node->busy = false;

	list_add_tail(&node->list, &identity_list);
	pr_debug("Added %s %d %d\n", node->name, node->id, node->busy);
	return 0;
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

static void identity_destroy(int id)
{
	struct identity *identity = identity_find(id);

	if (!identity) {
		pr_debug("Can't delete ID %d: not found.\n", id);
		return;
	}

	list_del(&identity->list);
	kfree(identity);
	pr_debug("Deleted ID %d\n", id);
}

static int __init init_m(void)
{
	pr_debug("Added Task12 module\n");

	if (identity_create("Alice", 1))
		pr_debug("Can't create Alice, ID: 1\n");

	if (identity_create("Bob", 2))
		pr_debug("Can't create Bob, ID: 2\n");

	if (identity_create("Dave", 3))
		pr_debug("Can't create Dave, ID: 3\n");

	if (identity_create("Gena", 10))
		pr_debug("Can't create Gena, ID: 10\n");

	struct identity *temp = identity_find(3);

	pr_debug("id 3 = %s\n", temp->name);

	temp = identity_find(42);
	if (temp == NULL)
		pr_debug("id 42 not found\n");

	identity_destroy(2);
	identity_destroy(1);
	identity_destroy(10);
	identity_destroy(42);
	identity_destroy(3);

	return 0;
}

static void __exit exit_m(void)
{
	pr_debug("Removed Task12 module\n");
}

module_init(init_m);
module_exit(exit_m);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Juan Antonio Aldea Armenteros");
MODULE_DESCRIPTION("Eudyptula Task 12");
