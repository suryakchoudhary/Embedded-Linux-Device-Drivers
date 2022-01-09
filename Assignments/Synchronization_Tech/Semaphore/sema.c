#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/semaphore.h>


#define NAME SYNCDRIV


MODULE_LICENSE("GPL");
MODULE_AUTHOR("SURYA");

struct cdev *my_cdev; 
struct semaphore sem;
struct completion data_read_done;
struct completion data_write_done;


//protocol
int NAME_open(struct inode *inode,struct file *filp);
ssize_t NAME_read(struct file *filp,char __user *ubuff,size_t count,loff_t *offp);
ssize_t NAME_write(struct file *filp, const char __user *ubuff,size_t count,loff_t *offp);
int NAME_release(struct inode *inode,struct file *filp);

struct file_operations fops=
{
	.owner   =   THIS_MODULE,
	.open    =   NAME_open,
	.read    =   NAME_read,
	.write   =   NAME_write,
	.release =   NAME_release,
};

dev_t DevNo;

static int __init prog_init(void)
{
	int result;
	int Maj, Min, RegDev;

	RegDev = alloc_chrdev_region(&DevNo, 0, 1, "SYNCDRIV");
//	DevNo = MKDEV(17,0);
	Maj = MAJOR(DevNo);
	Min = MINOR(DevNo);
	printk(KERN_INFO "\nMAJOR NUMBER = %d............MINOR NUMBER = %d..\n",Maj, Min);

	if(result<0)
	{
		printk(KERN_ALERT "\n Region not available\n");
		return(-1);
	}

	my_cdev = cdev_alloc();
	my_cdev->ops = &fops;

	result=cdev_add(my_cdev, DevNo, 1);

	if(result<0)
	{
		printk(KERN_INFO "\n THE DEVICE NUMBER AND CDEV NOT CONNECTED.\n");
		unregister_chrdev_region(DevNo, 1);
		return(-1);
	}

	// semaphore initialize
	sema_init(&sem,1);

	// completion initialize
	init_completion(&data_read_done);
	init_completion(&data_write_done);
	printk(KERN_ALERT "\n SEMAPHORE IS INITAILIZED..\n");
	return 0;
}

static void __exit prog_exit(void)
{
	int Maj, Min;

	Maj = MAJOR(DevNo);
	Min = MINOR(DevNo);
	cdev_del(my_cdev);
	unregister_chrdev_region(DevNo, 1);
	printk(KERN_INFO "\n THE MAJOR NUMBER %d.. THE MINOR NUMBER %d..\n",Maj, Min);
	printk(KERN_ALERT"\n Deallocated Everything\n");
}
module_init(prog_init);
module_exit(prog_exit);

//function definition

// Globally declared

char K_buff[60];

int NAME_open(struct inode *inode,struct file *filp)
{

	printk(KERN_ALERT "\n THE OPEN SYSTEM CALL IS CALLED...\n");
	return 0;
}

ssize_t NAME_read(struct file *filp,char __user *U_buff,size_t count,loff_t *offp)
{
	unsigned long result;
	ssize_t retval;
	wait_for_completion_interruptible(&data_write_done);
	result = copy_to_user((char*)U_buff, (char*)K_buff, count);
	complete(&data_read_done);
	if(result == 0)
	{
		printk(KERN_ALERT "\n MESSAGE TO USER..\n...%s....\n", K_buff);
		printk(KERN_INFO  "\n DATA SEND COMPLETED..\n");
		retval = count;
		return retval;
	}
	else if(result>0)
	{  
		printk(KERN_ALERT "\n MESSAGE TO USER..\n...%s....\n", K_buff);
		printk(KERN_ALERT "\n THE PART OF DATA IS SENDED..\n");
		retval = (count-result);
		return retval;
	}
	else
	{
		printk(KERN_ALERT "\n ERROR IN READING\n");
		retval = -EFAULT;
		return retval;
	}

}



ssize_t NAME_write(struct file *filp, const char __user *U_buff, size_t count, loff_t *offp)
{

	unsigned long result;
	ssize_t retval;
	down(&sem);
	printk(KERN_ALERT "\n CRITICAL SECTION STARTED..\n");

	result=copy_from_user((char*)K_buff, (char*)U_buff, count);
	complete(&data_write_done);
	wait_for_completion_interruptible(&data_read_done);
	up(&sem);

	if(result == 0)
	{
		printk(KERN_ALERT "\n MESSAGE FROM USER..\n...%s....\n", K_buff);
		printk(KERN_INFO  "\n DATA RECEIVED COMPLETED..\n");
		retval = count;
		return retval;
	}
	else if(result > 0)
	{  
		printk(KERN_ALERT "\n MESSAGE FROM USER..\n...%s....\n",K_buff);
		printk(KERN_ALERT "\n THE PART OF DATA IS RECEIVED..\n ");
		retval = (count-result);
		return retval;
	}
	else
	{
		printk(KERN_ALERT "\n ERROR IN WRITING\n");
		retval = -EFAULT;
		return retval;
	}

}
int NAME_release(struct inode *inode, struct file *filp)
{

	printk(KERN_ALERT "\n THE CLOSE SYSTEM CALL IS CALLED...\n");
	return 0;
}
