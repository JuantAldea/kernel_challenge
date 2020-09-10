#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init init_m(void)
{
	pr_debug("Hello World!\n");
	return 0;
}

static void __exit exit_m(void)
{
}

module_init(init_m);
module_exit(exit_m);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Juan Antonio Aldea Armenteros");
MODULE_DESCRIPTION("Prints hello world into the kernel log as debug message.");
