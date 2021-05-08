#ifndef CONSUMER_FILE
#define CONSUMER_FILE

#include <stdio.h>
#include "common.h"

typedef struct
{
	int pid;
	char sharedBufferName[50];
} consumerProcess; ///<

void tryRead(consumerProcess consumer);

#endif // CONSUMER_FILE