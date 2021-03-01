#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

int get_file_size()
{
	struct stat st;
	stat("07-1.c", &st);
	return st.st_size;
}

int main()
{
	char* array;
	int     shmid;
	int     new = 1;
	char    pathname[] = "07-1.c";
	key_t   key;
	int size = get_file_size();

	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if ((shmid = shmget(key, size * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
	{
		if (errno != EEXIST)
		{
			printf("Can\'t create shared memory\n");
			exit(-1);
		}
		else
		{
			if ((shmid = shmget(key, size * sizeof(char), 0)) < 0)
			{
				printf("Can\'t find shared memory\n");
				exit(-1);
			}
			new = 0;
		}
	}
	if ((array = (char*)shmat(shmid, NULL, 0)) == (int*)(-1))
	{
		printf("Can't attach shared memory\n");
		exit(-1);
	}
	if (new)
	{
		printf("Shared memory was empty.");
		exit(1);
	}
	else
	{
		printf("%s\n", array);
	}
	if (shmdt(array) < 0)
	{
		printf("Can't detach shared memory\n");
		exit(-1);
	}
	return 0;
}