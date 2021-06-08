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

int size = 31;

void si(pid_t sppid, int number)
{
	if (number & (1 << size))
	{
		kill(sppid, SIGUSR1);
	}
	else
	{
		kill(sppid, SIGUSR2);
	}
	size -= 1;
}

void handler1(int signal)
{
	if (size < 0)
	{
		printf("sended\n");
		exit(0);
	}
	si();
}

int main(void)
{
	pid_t sppid;
	int number = 0;

	printf("sender PID = %d\n", (int)getpid());
	printf("id: ");
	scanf("%d", &number);

	sppid = number;

	printf("sending count: \n");
	scanf("%d", &number);

	(void)signal(SIGUSR1, handler1);

	si(sppid, number);

	while (1) {}

	return 0;
}