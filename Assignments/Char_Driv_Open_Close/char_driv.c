//#include <linux/kdev_t.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SURYA");
MODULE_DESCRIPTION("DRIVER");

int CharDrivProg_open(struct inode *inode, struct file *filp);     //filp is the pointer to struct file
int CharDrivProg_release(struct inode *inode, struct file *filp);  //filp is the pointer to struct file

struct file_operations fops= //fops is a memory instance
{ 
	.open = CharDrivProg_open,
	.release = CharDrivProg_release,
};

//structure for the character driver
struct cdev *mychar_cdev;  //mychar_cdev is a pointer

//Struct that define the functionalities that driver provides
static int Char_Driver_init(void)
{
	dev_t DevNo;                                                                         
	int Major, Minor, Reg;

	DevNo = MKDEV(100, 0);       //Device number
	Major = MAJOR(DevNo);
	Minor = MINOR(DevNo);
	printk("\n Major number = %d ___________ Minor number = %d\n", Major, Minor);
	Reg = register_chrdev_region(DevNo, 1, "CharDeviceDriver");

	if(Reg < 0)
	{
		printk(KERN_ALERT "\n Region not available\n");
		return(-1);
	}

	mychar_cdev = cdev_alloc();
	mychar_cdev->ops = &fops;

	Reg = cdev_add(mychar_cdev, DevNo, 1);
	if (Reg < 0)
	{
		printk(KERN_ALERT "\nDevice number and file_operations struct not linked");
		unregister_chrdev_region(DevNo, 1);
		return(-1);
	}
	return 0;
}

static void Char_Driver_exit(void)
{
	dev_t DevNo;
	int Major, Minor, Reg;
	DevNo = MKDEV(100, 0);
	Major = MAJOR(DevNo);
	Minor = MINOR(DevNo);
	printk("\n Major number = %d ___________ Minor number = %d\n", Major, Minor);
	unregister_chrdev_region(DevNo, 1);
	cdev_del(mychar_cdev);
	printk(KERN_ALERT"\n Deallocated Everything\n");
}

int CharDrivProg_open(struct inode *inode, struct file *filp)
{
	printk(KERN_ALERT "\nOPENNNNNNNNNNNNNNNNNNNN CALLLLLLLLLLLLLLLLLLLLLLL\n");
	return 0;
}

int CharDrivProg_release(struct inode *inode, struct file *filp) //filp is the pointer to struct file
{
	printk(KERN_ALERT"\nCLOSEEEEEEEEEEEEEEEEEEEEEE CALLLLLLLLLLLLLLLLLLLLLLL\n");
	return 0;
}
module_init(Char_Driver_init);
module_exit(Char_Driver_exit);
