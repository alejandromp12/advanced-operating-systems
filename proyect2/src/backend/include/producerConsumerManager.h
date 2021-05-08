#ifndef PRODUCER_CONSUMER_MANAGER_FILE
#define PRODUCER_CONSUMER_MANAGER_FILE

#include <stdio.h>
#include "common.h"

typedef enum
{
	PRODUCER_ROLE = 0,
	CONSUMER_ROLE = 1
} producerConsumerRole;


typedef enum
{
	DECREASE= 0,
	INCREASE = 1
} producerConsumerAction;


int addProducerConsumer(producerConsumerRole role, char *bufferName);

int removeProducerConsumer(producerConsumerRole role, char *bufferName);

int getProducerConsumer(producerConsumerRole role, char *bufferName);

void logProducerConsumerAction(char *bufferName, producerConsumerRole role, int bufferIndex);

int getInActiveProducerConsumerPID(sharedBuffer *pSharedBuffer, producerConsumerRole role);

void setProducerConsumerPIDState(char *bufferName, int pid, producerConsumerPidState state, producerConsumerRole role);

void removeProducerConsumerPIDFromList(char *bufferName, int pid, producerConsumerRole role);

void insertProducerConsumerPIDToList(char *bufferName, int pid, producerConsumerRole role);

#endif // PRODUCER_CONSUMER_MANAGER_FILE