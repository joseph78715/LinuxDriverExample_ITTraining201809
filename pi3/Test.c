#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>

#define SIG_TEST 44 /* we define our own signal, hard coded since SIGRTMIN is different in user and in kernel space */ 


void receiveData(int n, siginfo_t *info, void *unused) 
{
    char *argv[] = { "/bin/sh", "-c", "shutdown -P -a -h now", 0 };
    char *envp[] =
    {
        "HOME=/",
        "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
        0
    };
     //printf("Received Signal from Kernel Module with value::%i\n", info->si_int);
    execve(argv[0], &argv[0], envp);
}

void main()
{
	char buf[10];
	int fp=-1;
      
	/* setup the signal handler for SIG_TEST 
 	 * SA_SIGINFO -> we want the signal handler function with 3 arguments
 	 */
	struct sigaction sig;
	sig.sa_sigaction = receiveData;
	sig.sa_flags = SA_SIGINFO;

	sigaction(SIG_TEST, &sig, NULL);
	memset(buf, 0, 100);
	fp =open("/dev/mydevice", O_RDWR);

	sprintf(buf, "%i", getpid());
	if (write(fp, buf, strlen(buf) + 1) < 0) {
		perror("fwrite"); 
		return -1;
	}

	if (!(read(fp, buf, strlen(buf), NULL)))
		printf("Read Success: \n");
	printf("The String is::%s\n", buf);
       
        return;	
}
