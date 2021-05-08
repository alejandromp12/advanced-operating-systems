#include "include/consumer.h"
#include "include/producerConsumerManager.h"
#include "include/scheduler.h"
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <semaphore.h>


int readData(bufferElement *pBuffElement, int bufferIndex, char *bufferName)
{
	int semValue;
    if ((pBuffElement->indexAvailable != 0) || (sem_getvalue(&(pBuffElement->mutex), &semValue) < 0) || (semValue == 0))
    {
		printf("Warning, not possible to read in the shared buffer.\n");
		return 0;
    }

    sem_wait(&(pBuffElement->mutex));

    //critical section
    printf("==================================\n");
	printf("Message READ at index %d of the buffer %s.\n", bufferIndex + 1, bufferName);
	printf("Producer ID who wrote the message %i.\n", pBuffElement->data.producerId);
	printf("Key of the message %i.\n", pBuffElement->data.key);
	printf("Date of the message %s.\n", pBuffElement->data.date);
	printf("==================================\n");

	//Cleaning?
	//pBuffElement->data.producerId = message.producerId;
    //strcpy(pBuffElement->data.date, message.date);
    //pBuffElement->data.key = message.key;

    // now available
    pBuffElement->indexAvailable = 1;

    //signal
    sem_post(&(pBuffElement->mutex));

    return 1;
}


void tryRead(consumerProcess *consumer)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(consumer ->sharedBufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return;
	}

	int index;
	int bufferSize = pSharedBuffer->size;
	for (consumer ->readIndex; consumer ->readIndex % bufferSize < bufferSize; consumer ->readIndex++)
	{
		index = consumer ->readIndex % bufferSize;
		
		if (!readData(&(pSharedBuffer->bufferElements[index]), index, consumer ->sharedBufferName))
		{
			if (index == (bufferSize - 1))
			{
				setProducerConsumerPIDState(consumer ->sharedBufferName, consumer ->pid, INACTIVE, CONSUMER_ROLE);
				doProcess(consumer ->pid, STOP);
				printf("CONSUMER with PID %i, just woke up.\n", consumer ->pid);
			}

			continue;
		}
		else
		{
			logProducerConsumerAction(consumer ->sharedBufferName, CONSUMER_ROLE, index);

			// wake up consumers
			wakeup(consumer ->sharedBufferName, PRODUCER_ROLE);
			consumer ->readIndex++; 

			return;
		}
	}
}