#ifndef FINISHER_FILE
#define FINISHER_FILE

#include <stdio.h>
#include "producerConsumerManager.h"

typedef enum
{
	REMOVE_SHARED_BUFFER = 0,
	WRITE_MSG_FOR_CONSUMERS = 1,
	WRITE_MSG_FOR_PRODUCERS = 2
} TerminatorLog;

int removeBuffer(char *bufferName);
int killProducers(char *bufferName);
int killConsumers(char *bufferName);
void logTerminatorAction(char *bufferName, int bufferIndex, bufferElement *pBuffElement, int whatToLog);

#endif // FINISHER_FILE