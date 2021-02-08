#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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
		// Запускаем другую программу - задание 3
		// Нужен уже скомпилированный файл... попытался сделать компиляцию + запуск, но не получилось...
		execv("03-3", argv);
		// На случай если запуск не удался
		printf("error!\n");
		exit(-1);
	}
	else
	{
		printf("I'm parent! I start in 03-4.c\n");
	}
	
	return 0;
}

