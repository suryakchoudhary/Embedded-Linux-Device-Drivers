/*2. Write a character driver with open, read, write and close functionalities
	• Test the driver through a user application by reading data from the driver and writing data to the driver*/

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/fs.h>
#include<linux/uaccess.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SURYA");
MODULE_DESCRIPTION("Device Driver All Operations");

int DrivAllOps_open(struct inode *inode, struct file *filp);
ssize_t DrivAllOps_read(struct file *filp,char __user *u_buff, size_t count, loff_t *offp);
ssize_t DrivAllOps_write(struct file *filp,const char __user *u_buff, size_t count, loff_t *offp);
int DrivAllOps_release(struct inode *inode, struct file *filp);

struct cdev *mychar_cdev;

struct file_operations fops =
{
	.open = DrivAllOps_open,
	.read = DrivAllOps_read,
	.write = DrivAllOps_write,
	.release = DrivAllOps_release,
};


static int __init Char_dev_init(void)
{
	dev_t DevNo;
	int Major, Minor, Reg;

	DevNo = MKDEV(100, 1);
	Major = MAJOR(DevNo);
	Minor = MINOR(DevNo);
	Reg = register_chrdev_region(DevNo, 1, "DrivAllOps");//Reg device no.
	printk(KERN_ALERT "\nDevice Number Registered With Kernel iiiiiinnnnniiiiiiiitttttt\n");
	if(Reg < 0)
	{
		printk(KERN_ALERT "\nDevice Number Not Registered With Kernel\n");
		return (-1);
	}

	mychar_cdev = cdev_alloc(); // <linux/cdve.h>
	mychar_cdev->ops = &fops;
 	Reg = cdev_add(mychar_cdev, DevNo, 1);
	if(Reg < 0)
	{
		printk(KERN_ALERT"\ncdev structure not added to kernel");
		unregister_chrdev_region(DevNo, 1);
		return(-1);
	}
	return 0;
}
static void __exit Char_dev_exit(void)
{
	dev_t DevNo;
	int Major, Minor, Reg;
	DevNo = MKDEV(100, 1);
	unregister_chrdev_region(DevNo, 1);
	cdev_del(mychar_cdev);
	printk(KERN_ALERT"\nDeallocated Everythinf Successfully\n");
}

int DrivAllOps_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT"\nOPENNNNNNNNNNNNNNNNNNNNNNN CALLLLLLLLLLLL\n");
	return 0;
}

// COPY TO USER
ssize_t DrivAllOps_read(struct file *filp, char __user *u_buff, size_t count, loff_t *offp)
{
	char K_buff[] = "Writing this char string from K_buff to U_buff";
	unsigned long to_user;
	ssize_t received;

	to_user = copy_to_user((char *)u_buff, (char *)K_buff, sizeof(K_buff));

//	put_user(to_user, K_buff);
	
	if(to_user == 0)
	{
		printk(KERN_ALERT"COPY TO USER\n");
		printk(KERN_ALERT"%s\n", u_buff);
		printk(KERN_ALERT"Got complete data\n");
		received = count;
		printk(KERN_ALERT"No. Of Bytes Received = %d\n", received);
		return (received);
	}
	else if(to_user > 0)
	{
		printk(KERN_ALERT"\nIncomplete Data Received at User\n %s", u_buff);
		received = (count-to_user);
		printk("\nNo. of bytes yet to Rev = %d\n", to_user);/////////////////////////////////////////
		return (received);
	}
	else
	{
		printk(KERN_ALERT"\nError in copy_to_user\n");
		received = -EFAULT;
		return(-1);
	}
}

// COPY FROM USER
ssize_t DrivAllOps_write(struct file *filp, const char __user *u_buff, size_t count, loff_t *offp)
{
	char K_buff[100];
	ssize_t received;
	unsigned long from_user;

	from_user = copy_from_user((char *)K_buff, (char *)u_buff, count);

//	get_user(from_user, K_buff);/////////////////////////////

	if(from_user == 0)
	{
		printk(KERN_ALERT"\nCOPY FROM USER\n");
		printk(KERN_ALERT"%s\n", K_buff);
		printk(KERN_ALERT"Got complete data from user\n");
		received = count;
		printk(KERN_ALERT"No. Of Bytes Received = %d\n", received);
		return (received);

	}
	else if(from_user > 0)
	{
		printk(KERN_ALERT"\nRECEIVED FROM USER = %s\n", K_buff);
		printk(KERN_ALERT"\nIncomplete data sent to Kernel\n");

		received = (count - from_user);
		printk("\nNo. of bytes need to be Rev = %d\n", received);
		return received;
	}
	else
	{
		printk(KERN_ALERT"\nError in copy from user\n");
		received = -EFAULT;
		return(-1);
	}
}
int DrivAllOps_release(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT"\nCLOSEEEEEEEEEEEEEEEEEEEEEEE CALLLLLLLLLLLLLLLL\n");
	return 0;
}


module_init(Char_dev_init);
module_exit(Char_dev_exit);


