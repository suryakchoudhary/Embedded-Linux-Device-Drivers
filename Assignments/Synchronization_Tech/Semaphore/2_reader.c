#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    int fd;
    char K_buff[60];
    ssize_t reading;
    
    fd=open("/dev/SYNCDRIV",O_RDWR,0777);

    if(fd < 0)
    {
        printf("ERROR IN OPENING..\n");
        exit(1);
    }
    reading = read(fd, K_buff, sizeof(K_buff));

    if(reading < 0)
    {
	   perror("Error in reading\n");
	   exit(EXIT_FAILURE);
    }
    printf("\nREADER_2 READING FROM KERNEL = %s\n", K_buff);
    printf("Byte written successfully = %ld\n", reading);
    close(fd);
    return 0;
}
