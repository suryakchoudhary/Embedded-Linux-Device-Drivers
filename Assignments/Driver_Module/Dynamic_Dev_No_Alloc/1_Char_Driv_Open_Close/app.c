/*1. Write a character driver with open and close functionality
	Test the driver by writing an application that opens and closes the device driver. When open or closed, the open and close calls in the driver should be executed.*/

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
	int fd1;
	//char Udata[]="User buff is sending data to kernel";
	//char Kdata[100];

	fd1 = open("/dev/CharDrivProg", O_RDWR);
	if (fd1 < 0)
	{
		printf("Error in opening Device\n");
		exit(1);
	}
	close(fd1);
	return 0;
}

