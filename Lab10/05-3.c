#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

/*
 N - количество циклов.
 Создаем key, sem, pipe.
 После fork получаем два процесса (Parent | Child):
 Parent:
	 for(N)
		 A(sem,1) -> записывает сообщение в pipe.
		 A(sem,2) -> проверяет sem -> чтение из pipe.

 Child:
	for(N)
		D(sem,2) -> читает из pipe.
		пишет в pipe -> D(sem,1)
 */

int main()
{
	char pathname[] = "05-3.c";
	int fd[2], result;
	int semid;
	key_t key;
	struct sembuf mybuf;
	char resstring[14];
	size_t size;

	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0)
	{
		printf("Can\'t create semaphore set\n");
		exit(-1);
	}

	int N;
	printf("N = ");
	scanf("%d", &N);

	// Создаем pipe
	if (pipe(fd) < 0)
	{
		printf("Can\'t open first pipe\n");
		exit(-1);
	}

	result = fork();

	if (result < 0)
	{
		printf("Can\'t fork child\n");
		exit(-1);
	}
	else if (result > 0)
	{
		/* Parent process */
		for (int i = 0; i < N; ++i)
		{
			mybuf.sem_num = 0;
			mybuf.sem_op = 1;
			mybuf.sem_flg = 0;

			if (semop(semid, &mybuf, 1) < 0)
			{
				printf("Can\'t add 1 to semaphore in parent\n");
				exit(-1);
			}

			size = write(fd[1], "Hello, world!", 14);

			if (size != 14)
			{
				printf("Can\'t write all string to pipe in parent\n");
				exit(-1);
			}

			mybuf.sem_num = 0;
			mybuf.sem_op = 2;
			mybuf.sem_flg = 0;

			if (semop(semid, &mybuf, 1) < 0)
			{
				printf("Can\'t add 2 to semaphore in parent\n");
				exit(-1);
			}

			mybuf.sem_num = 0;
			mybuf.sem_op = 0;
			mybuf.sem_flg = 0;

			if (semop(semid, &mybuf, 1) < 0)
			{
				printf("Can\'t wait for condition in parent\n");
				exit(-1);
			}

			// Читаем из pipe.
			size = read(fd[0], resstring, 14);

			if (size < 0)
			{
				printf("Can\'t read string from pipe in parent\n");
				exit(-1);
			}

			printf("Parent: %s\n", resstring);
		}

		if (close(fd[1]) < 0)
		{
			printf("Can\'t close writing side of pipe in parent\n");
			exit(-1);
		}

		if (close(fd[0]) < 0)
		{
			printf("Can\'t close reading side of pipe in parent\n");
			exit(-1);
		}

		printf("Parent end\n");
	}
	else
	{
		/* Child process */
		for (int i = 0; i < N; ++i)
		{
			mybuf.sem_num = 0;
			mybuf.sem_op = -2;
			mybuf.sem_flg = 0;

			if (semop(semid, &mybuf, 1) < 0)
			{
				printf("Can\'t decrease in child\n");
				exit(-1);
			}

			// Читаем из pipe.
			size = read(fd[0], resstring, 14);

			if (size < 0)
			{
				printf("Can\'t read string from pipe in child\n");
				exit(-1);
			}

			printf("child: %s\n", resstring);

			// Пишем в pipe.
			size = write(fd[1], "Hello, world!", 14);

			if (size != 14)
			{
				printf("Can\'t write all string to pipe in child\n");
				exit(-1);
			}

			mybuf.sem_num = 0;
			mybuf.sem_op = -1;
			mybuf.sem_flg = 0;

			if (semop(semid, &mybuf, 1) < 0)
			{
				printf("Can\'t decrease in child\n");
				exit(-1);
			}
		}

		if (close(fd[0]) < 0)
		{
			printf("Can\'t close reading side of pipe in child\n");
			exit(-1);
		}

		if (close(fd[1]) < 0)
		{
			printf("Can\'t close writing side of pipe in child\n");
			exit(-1);
		}

		printf("Child end\n");
	}

	return 0;
}
