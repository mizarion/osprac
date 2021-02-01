#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	pid_t pid, ppid, parent;

	parent = fork();

	pid = getpid();
	ppid = getppid();

	if (parent == -1)
	{
		printf("error\n");
	}
	else if (parent == 0)
	{
		printf("I'm child! my pid = %d, my ppid = %d\n", (int)pid, (int)ppid);
	}
	else
	{
		printf("I'm parent! My pid = %d, my ppid = %d\n", (int)pid, (int)ppid);
	}

	return 0;
}
