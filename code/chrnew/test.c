#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>

int main(int argc,char *argv[]){
	int dev_0 = open("/dev/mydev_1",O_RDWR);
	char buff[100];

	write(dev_0,"TEST DEV\n",10);

	int ret = read(dev_0 ,buff,32);

	printf("ret:%d\n", ret);
	printf("READ:%s\n", buff);

	close(dev_0);
	return 0;
}
