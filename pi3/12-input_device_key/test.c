#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>
#include <linux/input.h>

int main(int argc,char *argv[]){
  
	struct input_event ievent;
	int size = sizeof(struct input_event);
	int fd = open("/dev/input/event0",O_RDWR | O_NONBLOCK);
	int rd;
	
	if(fd < 0){
		printf("open event fail\n");
		return -1;
	}

	while(1){
	  
		memset((void*)&ievent , 0,size);
		rd = read(fd,(void*)&ievent,size);
		if(rd <= 0){
			sleep(1);
		}
		
		if(rd > 0 && ievent.type == EV_KEY){
			switch(ievent.code)
			{
				case BTN_0:
				ievent.value ? printf("button is 1\n") : printf("button is 0 \n");	
				break;
				
				default:
				printf("not support event code \n");
				break;
			}			
		}		
	}
	close(fd);
	return 0;
}