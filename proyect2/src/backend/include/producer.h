#ifndef PRODUCER_FILE
#define PRODUCER_FILE

#include <stdio.h>
#include "common.h"

typedef struct
{
	int pid;
	char sharedBufferName[50];
	int indexWrite;

} producerProcess; ///<

void tryWrite(dataMessage message, producerProcess *pProducer);

#endif // PRODUCER_FILE