#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>

char buff[10];

int main(int argc,char *argv[]){
	int i = 10;
	int res = 0;

	// clean the buff
	while(i--) buff[i] = 0; i = 0;


	printf("==========================\n");
	printf("START to NON_BLOCK READ FILE\n");
	// non-blocking io read:
	int fd = open("/dev/myBR_file", O_RDONLY | O_NONBLOCK );
	if(fd < 0){
		printf("can't open file myBR_file.\n");
	}else{
		res = read(fd,buff,10);
		if(res < 0){
			printf("### read function return nothing.\n");
		}else{
			printf("### read function return: %s\n",buff);
		}
	}
	close(fd);
	printf("==========================\n");



	printf("START to BLOCK READ FILE\n");
	// open file of the blocking IO device 
	fd = open("/dev/myBR_file", O_RDONLY);
	printf("start to test the blocking io\n");
	res = read(fd,buff,10);
	printf("data return: %s\n",buff);
	close(fd);
	printf("");
	return 0;
}
