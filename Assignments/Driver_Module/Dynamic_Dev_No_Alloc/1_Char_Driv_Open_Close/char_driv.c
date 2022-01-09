/*1. Write a character driver with open and close functionality
  Test the driver by writing an application that opens and closes the device driver. When open or closed, the open and close calls in the driver should be executed.*/

#include <linux/kdev_t.h>   	//  device numbers : Unknown declarations 
#include <linux/types.h>  	// For device numbers: Unknown declarations for kdev_t.h to use, corrected in kdev_t.h file also.
#include <linux/init.h>     	// ??????????????????????????????????????????????????
#include <linux/fs.h>       	// Registering and unregistering with device number, struct file_operations(.open, .release)
#include <linux/cdev.h>     	// Allocating and deleting cdev structure, Registering cdev
#include <linux/module.h>       // MODULE_LICENSE, MODULE_AUTHOR, MODULE_DESCRIPTION,Char_Driver_init, Char_Driver_exit, module_init(Char_Driver_init), module_exit(Char_Driver_exit)

#include <linux/kernel.h>

dev_t DevNo;

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
//	dev_t DevNo;                                                                         
	int Major, Minor, Reg, RegDev;

	RegDev = alloc_chrdev_region(&DevNo, 0, 1, "CharDeviceDriver");
	Major = MAJOR(DevNo);
	Minor = MINOR(DevNo);
	printk("\n Major number = %d ___________ Minor number = %d\n", Major, Minor);

	if(RegDev < 0)
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
//	dev_t DevNo;
	int Major, Minor, Reg;
	Major = MAJOR(DevNo);
	Minor = MINOR(DevNo);
	cdev_del(mychar_cdev);
	unregister_chrdev_region(DevNo,1);
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
