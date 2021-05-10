#ifndef CONSUMER_FILE
#define CONSUMER_FILE

#include <stdio.h>
#include <sys/time.h>
#include "common.h"

typedef struct
{
	int pid;
	char sharedBufferName[50];
	int readIndex;
	int readMessages;
	int killerPID;
	int consumerId;
	double timeSpendInMutex;
	double idleTime;
	struct timeval startTime;
} consumerProcess; ///<

void tryRead(consumerProcess *pConsumer);

#endif // CONSUMER_FILE