#include "include/producer.h"
#include "include/scheduler.h"
#include "include/producerConsumerManager.h"
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <semaphore.h>


int writeData(bufferElement *pBuffElement, dataMessage message, int bufferIndex, char *bufferName)
{
	int semValue;
    if ((pBuffElement->indexAvailable != 1) || (sem_getvalue(&(pBuffElement->mutex), &semValue) < 0) || (semValue == 0))
    {
    	printf("Warning, not possible to write in the shared buffer.\n");
    	return 0;
    }

    sem_wait(&(pBuffElement->mutex));

    //critical section
    pBuffElement->data.producerId = message.producerId;
    strcpy(pBuffElement->data.date, message.date);
    pBuffElement->data.key = message.key;

    // no longer available
    pBuffElement->indexAvailable = 0;

    printf("==================================\n");
	printf("Message WRITE at index %d of the buffer %s.\n", bufferIndex + 1, bufferName);
	printf("Producer ID who wrote the message %i.\n", pBuffElement->data.producerId);
	printf("Key of the message %i.\n", pBuffElement->data.key);
	printf("Date of the message %s.\n", pBuffElement->data.date);
	printf("==================================\n");

    //signal
    sem_post(&(pBuffElement->mutex));

    return 1;
}


void tryWrite(dataMessage message, producerProcess producer)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(producer.sharedBufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return;
	}

	int bufferSize = pSharedBuffer->size;
	for (int i = 0; i < bufferSize; i++)
	{
		if (!writeData(&(pSharedBuffer->bufferElements[i]), message, i, producer.sharedBufferName))
		{
			if (i == (bufferSize - 1))
			{
				setProducerConsumerPIDState(producer.sharedBufferName, producer.pid, INACTIVE, PRODUCER_ROLE);
				doProcess(producer.pid, STOP);
				i = 0;
				printf("PRODUCER with PID %i, just woke up.\n", producer.pid);
			}

			continue;
		}
		else
		{
			// perform logging of the process
			logProducerConsumerAction(producer.sharedBufferName, PRODUCER_ROLE, i);

			// wake up consumers
			wakeup(producer.sharedBufferName, CONSUMER_ROLE);
			return;
		}
	}
}