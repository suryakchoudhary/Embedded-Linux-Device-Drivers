#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/uaccess.h>
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/delay.h>
#include<linux/seqlock.h>
#include<linux/cdev.h>
#include<linux/slab.h>    //kmalloc()
#include<linux/device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SURYA");

//seqlock declaration

seqlock_t lock;
unsigned long global_var=0;
dev_t dev;
static struct cdev my_cdev;
static struct task_struct *kthread_1;
static struct task_struct *kthread_2;

//Function declaration:
int NAME_open(struct inode *inode,struct file *filp);
ssize_t NAME_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t NAME_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int NAME_release(struct inode *inode,struct file *filp);

//function of thread
int thread_function1(void *arg);
int thread_function2(void *arg);

int thread_function1(void *arg)
{
	while(!kthread_should_stop())
	{
		write_seqlock(&lock);
		global_var++;
		write_sequnlock(&lock);
		msleep(2000);
	}
	return 0;
}

int thread_function2(void *arg)
{
	unsigned int seq_no;
	unsigned long read_value;
	while(!kthread_should_stop())
	{
		do
		{
			seq_no=read_seqbegin(&lock);
			read_value=global_var;
		}while(read_seqretry(&lock,seq_no));
		printk("THREAD FUNCTION READ VALUE FUNCTION2 %lu\n",read_value);
		msleep(2000);

	}
	return 0;
}

struct file_operations fops=
{
	.owner  =   THIS_MODULE,
	.open   =   NAME_open,
	.read   =   NAME_read,
	.write  =   NAME_write,
	.release=   NAME_release,
};

int NAME_open(struct inode *inode,struct file *filp)
{
	printk("\n OPEN FUNCTION..\n");
	return 0;
}

int NAME_release(struct inode *inode,struct file *filp)
{
	printk("\n CLOSE FUNCTION..\n");
	return 0;
}

ssize_t NAME_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp)
{
	printk("\n READ FUNCTION..\n");
	return 0;
}

ssize_t NAME_write(struct file *filp,const char __user *ubuff,size_t count,loff_t *offp)
{
	printk("\n WRITE FUNCTION..\n");
	return count;
}   

static int __init prog_init(void)
{
	if((alloc_chrdev_region(&dev,28,1,"NAME"))<0)
	{
		printk("\n cannot create major number..\n");
		return -1;
	}
	// cdev structure

	cdev_init(&my_cdev,&fops);

	// addind cdev and major

	if((cdev_add(&my_cdev,dev,1))<0)
	{
		printk("\n cannot add cdev and major..\n");
		unregister_chrdev_region(dev,1);
		return -1;
	}

	// creating threads
	printk("\n creating threads..\n");
	kthread_1=kthread_run(thread_function1,NULL,"NAME_THREAD_1");
	if(kthread_1 !=NULL)
	{
		printk("\n Thread 1 created and running..\n");
	}
	else
	{
		printk("\n Thread 1 not created..\n");
		kthread_stop(kthread_1);
		return -1;
	}
	kthread_2=kthread_run(thread_function2,NULL,"NAME_THREAD_2");
	if(kthread_2 !=NULL)
	{
		printk("\n Thread 2 created and running..\n");
	}
	else
	{
		printk("\n Thread 2 not created..\n");
		kthread_stop(kthread_2);
		return -1;
	}
	seqlock_init(&lock);
	printk("\n DRIVER LOADED..\n");
	return 0;
}
static void __exit prog_exit(void)
{
	//stop the thread
	kthread_stop(kthread_1);
	kthread_stop(kthread_2);
	unregister_chrdev_region(dev,1);
	cdev_del(&my_cdev);
	printk("\n DRIVER UNLOADED..\n");
}
module_init(prog_init);
module_exit(prog_exit);
