#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SURYA");

static char * charvar = "Module";
static int intvar = 22;

module_param(charvar, charp, S_IRUGO);
module_param(intvar, int, S_IRUGO);

static int param_init(void)
{
	printk(KERN_ALERT "\nInside init Functionality\n");      //KERN_ALERT is highlighteing t$
	printk(KERN_ALERT "\nThe value of charvar is %s\n", charvar);
	return 0;
}

static void param_exit(void)
{
	printk(KERN_ALERT "\nExiting the module\n");
}

module_init(param_init);
module_exit(param_exit);
