#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define LAST_MESSAGE 255 // Message type for termination.

int main(void)
{
    int msqid;      // IPC descriptor for the message queue
    char pathname[]="Client.c"; // The file name used to generate the key.
                             // A file with this name must exist in the current directory.
    key_t  key;     // IPC key
    int i,len;      // Cycle counter and the length of the informative part of the message

    struct mymsgbuf // Custom structure for the message
    {
        long mtype;
        float mnumber;
    } mybuf;

    if ((key = ftok(pathname,0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }
    //
    // Trying to get access by key to the message queue, if it exists,
    // or create it, with read & write access for all users.
    //
    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get msqid\n");
        exit(-1);
    }

	int maxlen;
	while (1) {
		/* Server Receive */
		maxlen = sizeof(float);
		
		if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, maxlen, 0, 0)) < 0) {
		  printf("Can\'t receive mess from client\n");
		  exit(-1);
		}
		
		if (mybuf.mtype == LAST_MESSAGE) {
			msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
			return 0;
		}
		
		/* Server Send */
		
		//
		// Fill in the structure for the message and
		// determine the length of the informative part.
		//
		mybuf.mtype = 1;
		mybuf.mnumber = mybuf.mnumber * mybuf.mnumber;
		len = sizeof(mybuf.mnumber); //strlen(mybuf.mtext)+1;
		  
		//
		// Send the message. If there is an error,
		// report it and delete the message queue from the system.
		//
		if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
		  printf("Can\'t send mess to client\n");
		  msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
		  exit(-1);
		}
		
		sleep(100);
	}

	msgctl(msqid, IPC_RMID, (struct msqid_ds *)NULL);
    
    return 0;
}
