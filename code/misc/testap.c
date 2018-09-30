
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int	main(int argc, char *argv[])
{
	int	fd1;
	int n;
	char buf[128];

	printf("Start to open the device node.\n");
	fd1 = open("/dev/vmisc", O_RDWR);
	printf("Open device node OK. Get fd = %d\n", fd1);
    n=sprintf(buf,"%s","hello world");

	printf("write-->%s\n",buf);
	n=write(fd1,buf,n);
	
    n=read(fd1,buf,128);
    buf[n]=0;   
	printf("Read <--n=%d,%s\n\n",n,buf);

	close(fd1);
	return 0;
}
