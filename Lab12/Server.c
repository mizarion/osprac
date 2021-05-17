#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct mymsgbuf
{
	long mtype;
	float num;
} mybuf;

int main(void)
{
	char pathname[] = "Client.c";
	key_t  key;
	if ((key = ftok(pathname, 0)) < 0)
	{
		printf("Can\'t generate key\n");
		exit(-1);
	}

	int msqid;
	if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0)
	{
		printf("Can\'t get msqid\n");
		exit(-1);
	}

	int len, maxlen;
	while (1)
	{
		maxlen = sizeof(float);

		if ((len = msgrcv(msqid, (struct msgbuf*)&mybuf, maxlen, 0, 0)) < 0)
		{
			printf("Can\'t receive message from client\n");
			exit(-1);
		}

		if (mybuf.mtype == 255)
		{
			msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
			return 0;
		}

		mybuf.mtype = 1;
		mybuf.num = mybuf.num * mybuf.num;
		len = sizeof(mybuf.num);

		if (msgsnd(msqid, (struct msgbuf*)&mybuf, len, 0) < 0)
		{
			printf("Can\'t send message to client\n");
			msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
			exit(-1);
		}

		sleep(100);
	}

	msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);

	return 0;
}
