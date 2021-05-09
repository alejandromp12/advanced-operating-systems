#ifndef SCHEDULER_FILE
#define SCHEDULER_FILE

#include <stdio.h>
#include "producerConsumerManager.h"
#include "common.h"

typedef enum
{
	STOP = 0,
	CONTINUE = 1,
	KILL = 2
} processAction;

void doProcess(int pid, processAction action);

void wakeup(char *bufferName, producerConsumerRole role);

void wakeup2(sharedBuffer *pSharedBuffer, producerConsumerRole role);

#endif // SCHEDULER_FILE