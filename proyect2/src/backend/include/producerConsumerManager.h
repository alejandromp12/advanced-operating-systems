#ifndef PRODUCER_CONSUMER_MANAGER_FILE
#define PRODUCER_CONSUMER_MANAGER_FILE

#include <stdio.h>

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

#endif // PRODUCER_CONSUMER_MANAGER_FILE