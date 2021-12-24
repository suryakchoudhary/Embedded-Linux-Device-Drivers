# Header Files Associated With APIs

## dev_t MKDEV(int Major, int Minor)     <linux/types.h>
- Within the kernel it is used to hold device numbers—both the major and minor parts. 
- Pick any number which is not their in "cat /proc/devices"

## MAJOR(dev_t dev); & MINOR(dev_t dev);       <linux/kdev_t.h>  
- Used to take out the major and minor number from device number.

## Thus, for new drivers, we strongly suggest that you use dynamic allocation to obtain your major device number, rather than choosing a number randomly from the ones that are currently free. In other words, your drivers should almost certainly be using alloc_chrdev_region rather than register_chrdev_region.

### int register_chrdev_region(dev_t first, unsigned int count, char *name);   <linux/fs.h>
- Searching for the sequenced device number. register_chrdev_region will be 0 if the allocatio#n was successfully performed. In case of error, a negative error code will be returned.

### int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);
- Requesting kernel for the allocation of major number.

## void unregister_chrdev_region(dev_t first, unsigned int count);
- Regardless of how you allocate your device numbers, you should free them when they are no longer in use.