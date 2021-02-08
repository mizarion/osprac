#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void PrintInfo(char* argv[], char* envp[])
{
	printf("Args:\n");
	for (int i = 0; argv[i]; ++i)
	{
		printf("\targv[%d] = %s\n", i, argv[i]);
	}
	printf("Env:\n");
	for (int i = 0; envp[i]; ++i)
	{
		printf("\tenvp[%d] = %s\n", i, envp[i]);
	}
	printf("\n");
}

int main(int argc, char* argv[], char* envp[])
{
	int _fork = fork();
	if (_fork == -1)
	{
		printf("error\n");
		exit(-1);
	}
	else if (_fork == 0)
	{
		printf("\nParent:\n\n");
		PrintInfo(argv, envp);
	}
	else
	{
		printf("\nChild:\n\n");
		PrintInfo(argv, envp);
	}
	return 0;
}

