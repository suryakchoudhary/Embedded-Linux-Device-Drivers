# Day- 6   

## Allocating Device Number in Kernel

### Dynamic Allocation

- Requesting kernel for the allocation of major number.

- int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name);

- The disadvantage of dynamic assignment is that you can't create the device nodes in advance, because the major number assigned to your module will vary. For normal use of the driver, this is hardly a problem, because once the number has been assigned, you can read it from procdevices.

## Some Important Data Structures

### struct file_operations (file_operations structure)

- file_operations structure is initialized as follows:
-  struct file_operations scull_fops = {
-  .owner = THIS_MODULE,
-  .llseek = scull_llseek,
-  .read = scull_read,
-  .write = scull_write,
-  .ioctl = scull_ioctl,
-  .open = scull_open,
-  .release = scull_release,
-  };

- Most of the fundamental driver operations involve three important kernel data structures, called struct file_operations, struct file, and struct inode.

- The structure, defined in <linux/fs.h>, is a collection of function pointers. Each open file (represented internally by a file structure, which we will examine shortly) is associated with its own set of functions (by including a field called f_op that points to a file_operations structure). The operations are mostly in charge of implementing the system calls and are therefore, named open, read, and so on.

- Conventionally, a file_operations structure or a pointer to one is called fops (or some variation thereof ). Each field in the structure must point to the function in the driver that implements a specific operation, or be left NULL for unsupported operations.

#### APIs

- int (*open) (struct inode , struct file );
    - Though this is always the first operation performed on the device file, the driver is not required to declare a corresponding method. If this entry is NULL, opening the device always succeeds, but your driver isn't notified.

- ssize_t (*read) (struct file , char user , size_t, loff_t *);
    - Used to retrieve data from the device. A null pointer in this position causes the read system call to fail with -EINVAL ("Invalid argument"). A nonnegative return value represents the number of bytes successfully read (the return value is a "signed size" type, usually the native integer type for the target platform).

- ssize_t (*write) (struct file , const char user , size_t, loff_t*);
    - Sends data to the device. If NULL, -EINVAL is returned to the program calling the write system call. The return value, if nonnegative, represents the number of bytes successfully written.

- int (*release) (struct inode , struct file );
    - This operation is invoked when the file structure is being released. Like open, release can be NULL.

- int (*ioctl) (struct inode , struct file , unsigned int, unsigned long);
    - The ioctl system call offers a way to issue device-specific commands (such as formatting a track of a floppy disk, which is neither reading nor writing). Additionally, a few ioctl commands are recognized by the kernel without referring to the fops table. If the device doesn't provide an ioctl method, the system call returns an error for any request that isn't predefined (-ENOTTY, "No such ioctl for device").

- ssize_t (*aio_read)(struct kiocb , char user , size_t, loff_t);
    - Initiates an asynchronous read—a read operation that might not complete before the function returns. If this method is NULL, all operations will be processed (synchronously) by read instead.

- <linux/module.h>.
- loff_t (*llseek) (struct file *, loff_t, int); 
    - The llseek method is used to change the current read/write position in a file, and the new position is returned as a (positive) return value. The loff_t parameter is a "long offset" and is at least 64 bits wide even on 32-bit platforms. Errors are signaled by a negative return value.

### struct file

- The file structure represents an open file. It is not specific to device drivers; every open file in the system has an associated struct file in kernel space.

- struct file, defined in <linux/fs.h>, is the second most important data structure used in device drivers. Note that a file has nothing to do with the FILE pointers of userspace programs. A FILE is defined in the C library and never appears in kernel code. 

- A struct file, on the other hand, is a kernel structure that never appears in user programs.

- It is created by the kernel on open and is passed to any function that operates on the file, until the last close. After all instances of the file are closed, the kernel releases the data structure.

- In the kernel sources, a pointer to struct file is usually called either file or filp ("file pointer"). We'll consistently call the pointer filp to prevent ambiguities with the structure itself. Thus, file refers to the structure and filp to a pointer to the structure.

#### APIs

- mode_t f_mode;
    - The file mode identifies the file as either readable or writable (or both), by means of the bits FMODE_READ and FMODE_WRITE. You might want to check this field for read/write permission in your open or ioctl function, but you don't need to check permissions for read and write, because the kernel checks before invoking your method.

- loff_t f_pos;
    - The current reading or writing position. loff_t is a 64-bit value on all platforms (long long in gcc terminology). The driver can read this value if it needs to know the current position in the file but should not normally change it; read and write should update a position using the pointer they receive as the last argument instead of acting on filp->f_pos directly. The one exception to this rule is in the llseek method, the purpose of which is to change the file position.

- unsigned int f_flags; <linux/fcntl.h>
    - These are the file flags, such as O_RDONLY, O_NONBLOCK, and O_SYNC. A driver should check the O_NONBLOCK flag to see if nonblocking operation has been requested (we discuss nonblocking I/O in Section 6.2.3); the other flags are seldom used. In particular, read/write permission should be checked using f_mode rather than f_flags. All the flags are defined in the header <linux/fcntl.h>.

- struct file_operations *f_op;                            ?????????????????????????????????????????????????
    - The operations associated with the file.

- void *private_data;
    - private_data is a useful resource for preserving state information across system calls and is used by most of our sample modules.

- struct dentry *f_dentry;                                 ??????????????????????????????????????????????

### struct inode

- The inode structure is used by the kernel internally to represent files. Therefore, it is different from the file structure that represents an open file descriptor. There can be numerous file structures representing multiple open descriptors on a single file, but they all point to a single inode structure.

#### APIs

- dev_t i_rdev;
    - For inodes that represent device files, this field contains the actual device number.
    - The type of i_rdev changed over the course of the 2.5 development series,
    - breaking a lot of drivers. As a way of encouraging more portable programming, the kernel developers have added two macros that can be used to obtain the major and minor number from an inode:
    
    - unsigned int iminor(struct inode *inode);
    - unsigned int imajor(struct inode *inode);

- struct cdev *i_cdev;
    - struct cdev is the kernel's internal structure that represents char devices; this field contains a pointer to that structure when the inode refers to a char device file.