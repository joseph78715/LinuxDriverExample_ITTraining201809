#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int sim_fd, x, y;

	sim_fd = open("/sys/devices/platform/vms/coordinates", O_RDWR);
	if (sim_fd < 0) {
		printf("open vms coordinates failed");
		return 1;
	}

	printf("start sending x,y\n");
	while(1) {
		char buf[10];

		x = random()%50;
		y = random()%50;
		if(x%2) x = -x;
		if(y%2) y = -y;

		sprintf(buf, "%d %d %d", x, y, 0);
		write(sim_fd, buf, strlen(buf));
		fsync(sim_fd);
		sleep(1); 
	}

	close(sim_fd);

	return 0;
}