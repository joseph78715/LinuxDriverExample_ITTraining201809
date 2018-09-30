#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<time.h>

int fd = 0;
int accept_flag = 0;
int buf[30] ;
char dev_name[] = "/dev/mydevice";

void signal_handler(int num);

/*    //增加檔的某個flags，比如檔是阻塞的，想設置成非阻塞:
      flags = fcntl(fd,F_GETFL,0);
      flags |= O_NONBLOCK;
      fcntl(fd,F_SETFL,flags);
*/
 
int main(int argc,char *argv[]){


	// setup this process that 
	// if accept a signal: SIGIO
	// call func: signal_handler()
	signal(SIGIO, signal_handler);

	int pid = getpid();
	printf("process with pid %d START!\n", pid);

	fd = open(dev_name ,O_RDONLY);
	if(fd <= 0)	{
		printf("open device fail \n");
		return 0;
	}	
		

	printf("process open device %s with fd: %d\n", dev_name , fd);
	/*
	int fcntl(int fd, int cmd, ...  arg  ) 
	*/	
	//https://www.safaribooksonline.com/library/view/linux-device-drivers/0596000081/ch05s04.html
	/*
	First, they specify a process as the “owner” of the file. 
	When a process invokes the F_SETOWN command using the fcntl system call,
	the process ID of the owner process is saved in filp->f_owner for later use.
	This step is necessary for the kernel to know just who to notify. 
	*/
	fcntl(fd, F_SETOWN, getpid());	
	
	/* When F_SETFL is executed to turn on FASYNC, the driver’s fasync method is called. */
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | FASYNC); 

	
	while(!accept_flag){
		printf("Nothing to do ... Sleeping ... ZZZzzz....\n");
		usleep(1000000);
	}

	printf("I accepted something...and work done!\n");
	printf("Now the process would exit....\n");
	close(fd);
	return 0;
}

void signal_handler(int num){
	printf("accept signal which means the data ready for read:\n");
	printf("So, I read it !\n");
	int len = read(fd, buf, 30);
	buf[5] = '\0';
	printf("msg with len : %d\n", len);
	printf("I accept : %s\n", buf);
	while(len--) buf[len] = '\0';
	accept_flag = 1;
}
