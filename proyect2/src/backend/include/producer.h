#ifndef PRODUCER_FILE
#define PRODUCER_FILE

#include <stdio.h>
#include "common.h"

typedef struct
{
	int pid;
	char sharedBufferName[50];
	int indexWrite;
	int writtenMessage;

} producerProcess; ///<

void tryWrite(dataMessage message, producerProcess *pProducer);
void logProducerTermination(producerProcess *pProducer);

#endif // PRODUCER_FILE