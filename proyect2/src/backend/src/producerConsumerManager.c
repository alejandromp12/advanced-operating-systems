#include "include/producerConsumerManager.h"
#include "include/common.h"

int getProducerConsumer(producerConsumerRole role, char *bufferName)
{
	// will be overwritten if needed
	int count = -1;

	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return count;
	}

	switch (role)
	{
		case PRODUCER_ROLE:
		{
			sem_wait(&(pSharedBuffer->counter.producersMutex));
			count = pSharedBuffer->counter.producers;
			sem_post(&(pSharedBuffer->counter.producersMutex));
			break;
		}

		case CONSUMER_ROLE:
		{
			sem_wait(&(pSharedBuffer->counter.consumersMutex));
			count = pSharedBuffer->counter.consumers;
			sem_post(&(pSharedBuffer->counter.consumersMutex));
			break;
		}

		default:
			break;
	}

	return count;
}


int doActionToCounter(sharedBuffer *pSharedBuffer, producerConsumerRole role, producerConsumerAction action)
{
	int result = -1;
	switch (role)
	{
		case PRODUCER_ROLE:
		{
			sem_wait(&(pSharedBuffer->counter.producersMutex));

			if (action == INCREASE)
			{
				++(pSharedBuffer->counter.producers);
			}
			else if ((action == DECREASE) && (pSharedBuffer->counter.producers > 0))
			{
				--(pSharedBuffer->counter.producers);
			}
			
			result = pSharedBuffer->counter.producers;

			sem_post(&(pSharedBuffer->counter.producersMutex));

			break;
		}

		case CONSUMER_ROLE:
		{
			sem_wait(&(pSharedBuffer->counter.consumersMutex));

			if (action == INCREASE)
			{
				++(pSharedBuffer->counter.consumers);
			}
			else if ((action == DECREASE) && (pSharedBuffer->counter.consumers > 0))
			{
				--(pSharedBuffer->counter.consumers);
			}

			result = pSharedBuffer->counter.consumers;

			sem_post(&(pSharedBuffer->counter.consumersMutex));

			break;
		}

		default:
			break;
	}

	return result;
}


int addProducerConsumer(producerConsumerRole role, char *bufferName)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return -1;
	}

	return doActionToCounter(pSharedBuffer, role, INCREASE);
}

int removeProducerConsumer(producerConsumerRole role, char *bufferName)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return -1;
	}

	return doActionToCounter(pSharedBuffer, role, DECREASE);
}


void logProducerConsumerAction(char *bufferName, producerConsumerRole role, int bufferIndex)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return;
	}

	// perform logging of the process
	char log[200];
	sprintf(log,
		    "==================================\n"
		    "Message %s at index %d of the buffer %s\n"
		    "Consumers alive: %d\n"
		    "Producers alive: %d\n"
		    "==================================",
			(role == PRODUCER_ROLE) ? "produced" : "consumed",
			bufferIndex,
			bufferName,
			getProducerConsumer(CONSUMER_ROLE, bufferName),
			getProducerConsumer(PRODUCER_ROLE, bufferName));

	doLogging(log, pSharedBuffer->counter);

	//printf("Message logged:\n%s\n", log);
}