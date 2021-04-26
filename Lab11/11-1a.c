#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void)
{
	int msqid;      // IPC descriptor for the message queue
	char pathname[] = "11-1a.c"; // The file name used to generate the key.
							   // A file with this name must exist in the current directory.
	key_t  key;     // IPC key
	int i, len;      // Cycle counter and the length of the informative part of the message

	struct mymsgbuf
	{
		long mtype;
		struct
		{
			short sinfo;
			float finfo;
		} info;
	} mybuf;

	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}

	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
	{
		printf("Can\'t get msqid\n");
		exit(-1);
	}

	/* Send information */

	for (i = 1; i <= 5; i++)
	{
		mybuf.mtype = 1;
		mybuf.info.sinfo = 123;
		mybuf.info.sinfo = 123;

		len = sizeof(mybuf.info);

		if (msgsnd(msqid, (struct msgbuf*)&mybuf, len, 0) < 0)
		{
			printf("Can\'t send message to queue\n");
			msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
			exit(-1);
		}
	}

	/* Send the last message */

	mybuf.mtype = LAST_MESSAGE;
	len = 0;

	if (msgsnd(msqid, (struct msgbuf*)&mybuf, len, 0) < 0)
	{
		printf("Can\'t send message to queue\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
		exit(-1);
	}

	return 0;
}
