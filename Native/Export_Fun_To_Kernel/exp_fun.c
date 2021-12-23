#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("SURYA");

int Add(int a, int b)
{
        return (a+b);
}

EXPORT_SYMBOL_GPL(Add);

static int fun_init(void)
{
        printk(KERN_ALERT "\nExporting Symbol\n");
        return 0;
}

static void fun_exit(void)
{
        printk(KERN_ALERT "\nSymbol Cleanup\n");
}

module_init(fun_init);
module_exit(fun_exit);
