
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>

int main()
{
 int fd;
 char buf[1024];
 int r;
 fd=open("/proc/driver/myproc",O_RDWR);
 assert(fd>0);
 while ((r= read(fd,buf,1))>0) {
	 
 buf[r]='\0';
 close(fd);
 printf("r=%d,%s\n",r,buf);
 }

 return 0;
}

