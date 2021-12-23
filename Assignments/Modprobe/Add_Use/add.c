#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include "kern_add.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SURYA");

static int adding_init(void)
{
	printk("Addition = %d\n", Add(2,6));
	return 0;
}

static void adding_exit(void)
{
	printk("Result Displayed\n");
}
module_init(adding_init);
module_exit(adding_exit);
