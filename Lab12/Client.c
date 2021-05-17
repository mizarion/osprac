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

	float n;
	printf("Input: ");
	scanf("%f", &n);

	mybuf.mtype = 1;
	mybuf.num = n;
	int len = sizeof(mybuf.num);

	if (msgsnd(msqid, (struct msgbuf*)&mybuf, len, 0) < 0)
	{
		printf("Can\'t send message to server\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
		exit(-1);
	}

	int maxlen = sizeof(float);
	if ((len = msgrcv(msqid, (struct msgbuf*)&mybuf, maxlen, 1, 0)) < 0)
	{
		printf("Can\'t receive message from server\n");
		exit(-1);
	}
	printf("type = %ld, info = %f\n", mybuf.mtype, mybuf.num);

	mybuf.mtype = 255;
	len = 0;

	if (msgsnd(msqid, (struct msgbuf*)&mybuf, len, 0) < 0)
	{
		printf("Can\'t send message to server\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
		exit(-1);
	}

	msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);

	return 0;
}
