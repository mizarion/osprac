#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LAST_MESSAGE 255

int main(void)
{
    int msqid;
    char pathname[]="Client.c";
    key_t  key;
    int len, maxlen;

    struct mymsgbuf
    {
        long mtype;
        float mnumber;
    } mybuf;

    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

	/* Read message. */
	float message;
	printf("Input >> ");
	scanf("%f", &message);

	/* Send to server. */
	
	//
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    mybuf.mtype = 1;
    mybuf.mnumber = message;
    len = sizeof(mybuf.mnumber);
      
    //
    // Send the message. If there is an error,
    // report it and delete the message queue from the system.
    //
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send mess to server\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

	/* Receive */
	maxlen = sizeof(float);

    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 1, 0)) < 0) {
      printf("Can\'t receive message from server\n");
      exit(-1);
    }
    printf("type = %ld, info = %f\n", mybuf.mtype, mybuf.mnumber);

	/* LAST MESSAGE */
	mybuf.mtype = LAST_MESSAGE;
	len = 0;

	if (msgsnd(msqid, (struct msgbuf*)&mybuf, len, 0) < 0) {
		printf("Can\'t send mess to server\n");
		msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
		exit(-1);
	}

    msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);

    return 0;
}
