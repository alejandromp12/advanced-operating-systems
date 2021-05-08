#ifndef CONSUMER_FILE
#define CONSUMER_FILE

#include <stdio.h>
#include "common.h"

typedef struct
{
	int pid;
	char sharedBufferName[50];
	int readIndex;
	int readMessage;

} consumerProcess; ///<

void tryRead(consumerProcess *pConsumer);
int readData(bufferElement *pBuffElement, int bufferIndex, char *bufferName, consumerProcess *pConsumer);
void logConsumerTermination(consumerProcess *pConsumer, int key);
void xkillConsumer(consumerProcess *pConsumer);

#endif // CONSUMER_FILE