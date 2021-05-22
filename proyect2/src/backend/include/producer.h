#ifndef PRODUCER_FILE
#define PRODUCER_FILE

#include <stdio.h>
#include <sys/time.h>
#include "common.h"

typedef struct
{
	int pid;
	char sharedBufferName[50];
	int indexWrite;
	int writtenMessages;
	int producerId;
	double timeSpendInMutex;
	double idleTime;
	struct timeval startTime;
} producerProcess; ///<

void tryWrite(dataMessage message, producerProcess *pProducer);

#endif // PRODUCER_FILE