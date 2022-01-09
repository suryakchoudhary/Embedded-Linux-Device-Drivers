#include<stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdlib.h>
#include <errno.h>
#include <unistd.h>

int main()
{
	int fd;
	ssize_t reading, writing;
	char U_buff[50] = "Writing this string from user size";
	char K_buff[100];

	fd = open("/dev/DrivAllOps", O_RDWR, 0777);
	if(fd < 0)
	{
		printf("Error in opening device\n");
		exit(1);
	}

	//READING: COPY FROM KERNEL BUFFER
	reading = read(fd, K_buff, 45);
	if (reading < 0)
	{
		perror("Error in reading\n");
		exit(EXIT_FAILURE);
	}
	printf("Byte written successfully = %ld\n", reading);
	printf("READ FROM KERNEL = %s\n\n\n", K_buff);

	//WRITING: COPY TO KERNEL BUFFER
	writing = write(fd, U_buff, sizeof(U_buff));
	if (writing < 0 )
	{
		perror("Error in reading\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		printf("Byte written successfully = %ld\n", writing);
		printf("COPY FROM USER TO KERNEL = %s\n", U_buff);
	}
	close(fd);
}

	

