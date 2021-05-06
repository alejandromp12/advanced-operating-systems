#include "include/productConsumerManager.h"
#include "include/common.h"

int getProductConsumers(productConsumerRole role, char *bufferName)
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


int doActionToCounter(productConsumer counter, productConsumerRole role, productConsumerAction action)
{
	int result = -1;
	switch (role)
	{
		case PRODUCER_ROLE:
		{
			sem_wait(&counter.producersMutex);

			if (action == INCREASE)
			{
				++counter.producers;
			}
			else if ((action == DECREASE) && (counter.producers > 0))
			{
				--counter.producers;
			}
			
			result = counter.producers;

			sem_post(&counter.producersMutex);

			break;
		}

		case CONSUMER_ROLE:
		{
			sem_wait(&counter.consumersMutex);

			if (action == INCREASE)
			{
				++counter.consumers;
			}
			else if ((action == DECREASE) && (counter.consumers > 0))
			{
				--counter.consumers;
			}

			result = counter.consumers;

			sem_post(&counter.consumersMutex);

			break;
		}

		default:
			break;
	}

	return result;
}


int addProductConsumer(productConsumerRole role, char *bufferName)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return -1;
	}

	return doActionToCounter(pSharedBuffer->counter, role, INCREASE);
}

int removeProductConsumer(productConsumerRole role, char *bufferName)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return -1;
	}

	return doActionToCounter(pSharedBuffer->counter, role, DECREASE);
}


void logProductConsumerAction(char *bufferName, productConsumerRole role, int bufferIndex)
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
			getProductConsumers(CONSUMER_ROLE, bufferName),
			getProductConsumers(PRODUCER_ROLE, bufferName));

	doLogging(log, pSharedBuffer->counter);

	//printf("Message logged:\n%s\n", log);
}