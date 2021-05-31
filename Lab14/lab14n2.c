#include <signal.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <errno.h>
#include <stdlib.h>

void f(int nsig)
{
	switch (nsig)
	{
	case SIGINT:
		printf("ctrl + C was ignored");
		break;
	case SIGQUIT:
		printf("ctrl + 4 was ignored");
		break;
	default:
		break;
	}
}

int main(void)
{
	(void)signal(SIGINT, f);	// press ctrl + c.
	(void)signal(SIGQUIT, f);	// press ctrl + 4.


	while (1);
	return 0;
}
