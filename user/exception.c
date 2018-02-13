#define _GNU_SOURCE

#include "../kernel/main.h"
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>

unsigned long target;

struct ex_handler
{
	unsigned short cs;
	unsigned long pc;
} __attribute__((packed));

unsigned short read_cs()
{
	unsigned short ret;

	asm volatile("movw %%cs, %0\n\t":"=r" (ret));
	return ret;
}

extern void __ex_handler(void);

int main(int argc, char **argv)
{
	struct ex_handler h;
	char name[100];
	int fd;
	int c = 100;

	h.cs = read_cs();
	h.pc = (unsigned long)__ex_handler;

	snprintf(name, sizeof(name), "/dev/%s", MODULE_NAME);

	fd = open(name, O_RDONLY);
	if (fd < 0)
	{
		perror("unable to open dev");
		return 0;
	}

	if (ioctl(fd, REGISTER_HANDLER, &h) != 0)
	{
		perror("ioctl COUNT_DIVBYZERO error");
		abort();
	}

	c =  1/0;
	printf("Result of divide by zero is %d\n", c);

	if (ioctl(fd, UNREGISTER_HANDLER, NULL) != 0)
	{
		perror("ioctl COUNT_DIVBYZERO error");
		abort();
	}

	close(fd);

	return 0;
}
