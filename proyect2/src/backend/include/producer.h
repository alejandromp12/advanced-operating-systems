#ifndef PRODUCER_FILE
#define PRODUCER_FILE

#include <stdio.h>
#include "common.h"

typedef struct
{
	int pid;
	char sharedBufferName[50];
} producerProcess; ///<

void tryWrite(dataMessage message);

producerProcess _producer;

#endif // PRODUCER_FILE