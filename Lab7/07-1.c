#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int get_file_size()
{
	struct stat st;
	stat("07-1.c", &st);
	return st.st_size;
}

int main()
{
	char* array;    // Pointer to shared memory
	int     shmid;     // IPC descriptor for the shared memory
	int     new = 1;   // Initialization flag for the elements of the array
	char    pathname[] = "07-1.c"; // The file name used to generate the key.
						   // A file with this name must exist in the current directory.
	key_t   key;       // IPC key
	int size = get_file_size();
	//
	// Generate an IPC key from the file name 07-1a.c in the current directory
	// and the instance number of the shared memory 0.
	//
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
			printf("Can\'t find shared memory\n");
			exit(-1);
		}
	}
	//
	// Try to map shared memory to the address space of the current process.
	// Note that for proper comparison we explicitly convert the value -1 to a pointer to an integer.
	//
	if ((array = (char*)shmat(shmid, NULL, 0)) == (int*)(-1))
	{
		printf("Can't attach shared memory\n");
		exit(-1);
	}

	int fs = open("07-1.c", O_RDONLY);
	if (fs < 0)
	{
		printf("Can't open file");
		exit(-1);
	}
	int k = 0;
	while (read(fs, &array[k], 1) > 0)
	{
		k++;
	}
	array[k + 1] = EOF;
	if (close(fs) < 0)
	{
		printf("Can't close file");
	}
	if (shmdt(array) < 0)
	{
		printf("Can't detach shared memory\n");
		exit(-1);
	}

	return 0;
}

