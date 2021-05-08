#ifndef CONSUMER_FILE
#define CONSUMER_FILE

#include <stdio.h>
#include "common.h"

typedef struct
{
	int pid;
	char sharedBufferName[50];
	int readIndex;
} consumerProcess; ///<

void tryRead(consumerProcess *pConsumer);

#endif // CONSUMER_FILE