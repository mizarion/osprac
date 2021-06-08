#include <sys/types.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

pid_t sppid;
int size = 31;
int result = 0;

void check()
{
	kill(sppid, SIGUSR1);
	printf("result = %d\n", result);
	if (size < 0)
	{
		exit(0);
	}
}

void handler1(int signal_value)
{
	result += 1 << size;
	size -= 1;
	check();
}

void handler2(int signal_value)
{
	if (sppid > 0)
	{
		size -= 1;
		check();
	}
}

int main(void)
{
	(void)signal(SIGUSR1, handler1);
	(void)signal(SIGUSR2, handler2);

	printf("get pid = %d\n", (int)getpid());
	printf("input id: ");
	int number = 0;
	scanf("%d", &number);
	sppid = number;
	while (1) {}
	return 0;
}