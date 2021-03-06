# Kernel Utilities

- insmod file_name.ko : Inserting module to kernel one by one.

- rmmod file_name : To remove module to kernel one by one.

- lsmod : To view list the modules present in kernel (in sudo -s mode).

- dmesg : To display message (in sudo -s mode).

- tail /var/log/syslog : To check loaded modules to kernel.

- cat /proc/kallsyms | grep file_name

- cat /proc/devices : To display Major device numbers(driver associated with device) associated with the devices.

- ls -al /dev/ : Device files

- sudo mknod /dev/MyCharDevice c 100 0 : To create device file/node file

- sudo rm mknod /dev/MyCharDevice c 100 0

# Modprobe Commands

- Step 1: make

- Step 2: sudo make install : To copy .ko file to /lib/modules/$(uname -r)/extra/

- Step 3: sudo modprobe -i file_name: To load all the modules on which file_name is dependent by checking the dependencies in modules.dep file (which is present at /lib/modules/$(uname -r)/ location)

- depmode -a : Present in the "install:" section in "Makefile" which updates the dependency file(modules.dep file hich is present at /lib/modules/$(uname -r)/ location).

- Step 4: dmesg -c : To display running functionality only inside the kernel.

- Step 5: sudo modprobe -r file_name : This command will automatically unload all the file_name module and as well as modules on which file_name is dependent.

- Sequence will be 1st all the modules will be loaded on which file_name module is dependent, all these dependent modules(module symbols) will also be loaded according to its sequence of dependency.

# Character Driver Commands

- Step 1: Create .ko file for kernal module and executable for user source code.

- Step 2: Load the .ko file to the kernal by "sudo insmod file.ko".

- Step 3: Now run the user application source code in the sudo mode by "sudo ./executable".

- Step 4: Check the result either by "dmesg -c" or by "tail /var/log/syslog".

# Linux Commands

- ls -lh : For checking file size and other details.

- rm -r ditectory_name : To delete directory.


