#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdlib.h>
#include <errno.h>
#include <unistd.h>

int main(int argc,const char *argv[])
{
	int fd;
	char U_buff[60]="THIS IS FROM USER SPACE BY WRITER 1..\n";
	ssize_t writing;

	fd = open("/dev/SYNCDRIV", O_RDWR, 0777);
	if(fd < 0)
	{
		printf("ERROR IN OPENING DEVICE..\n");
		exit(1);
	}

	writing = write(fd, U_buff, sizeof(U_buff));
	if (writing < 0 )
	{
		perror("Error in writing\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("WRITER_1 WRITING FROM USER TO KERNEL = %s\n", U_buff);
		printf("\nByte written successfully = %ld\n", writing);
	}
	close(fd);
}
