#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void)
{
	char oldName[12];
	char newName[12];
	FILE* w;
	FILE* r;

	int counter = 0;
	sprintf(oldName, "%d", counter);
	w = fopen(oldName, "w");
	fclose(w);
	r = fopen(oldName, "r");
	fclose(r);
	++counter;

	while (1)
	{
		sprintf(newName, "%d", counter);
		w = fopen(newName, "w");
		fclose(w);
		if (symlink(oldName, newName) == -1)
		{
			break;
		}
		r = fopen(newName, "r");
		fclose(r);
		strcpy(oldName, newName);
		++counter;
	}
	printf("Req level: %d", counter);

	return 0;
}
