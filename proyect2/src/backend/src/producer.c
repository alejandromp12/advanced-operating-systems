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
	
	pBuffElement->data.killFlag = NO_PID;

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

void logProducerTermination(producerProcess *pProducer)
{
	printf("==================================\n");
	printf("Producer with pid %i killed due to termination flag\n", pProducer ->pid);
	printf("Producer putted %i messages in the mailbox\n", pProducer ->writtenMessage);
	printf("==================================\n");
}

void xkillProducer(producerProcess *pProducer)
{
	removeProducerConsumer(PRODUCER_ROLE, pProducer ->sharedBufferName);
	removeProducerConsumerPIDFromList(pProducer ->sharedBufferName, pProducer ->pid, PRODUCER_ROLE);
	logProducerTermination(pProducer);
	exit(0);
}

void tryWrite(dataMessage message, producerProcess *pProducer)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(pProducer->sharedBufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return;
	}

	if(pSharedBuffer ->killFlag == TERMINATE_SYSTEM) xkillProducer(pProducer);

	int index;
	int bufferSize = pSharedBuffer->size;
	for (pProducer->indexWrite; (pProducer->indexWrite % bufferSize) < bufferSize; pProducer->indexWrite++)
	{	
		index = pProducer->indexWrite % bufferSize;
		if (!writeData(&(pSharedBuffer->bufferElements[index]), message, index, pProducer->sharedBufferName))
		{	
			if(pSharedBuffer ->killFlag == TERMINATE_SYSTEM) 
			{
				xkillProducer(pProducer);
			}

			if (isBufferFull(pSharedBuffer))
			{
				setProducerConsumerPIDState(pProducer->sharedBufferName, pProducer->pid, INACTIVE, PRODUCER_ROLE);

				wakeup(pProducer->sharedBufferName, CONSUMER_ROLE);
				doProcess(pProducer->pid, STOP);
				
				printf("PRODUCER with PID %i, just woke up.\n", pProducer->pid);
			}

			continue;

		}
		else
		{
			// perform logging of the process
			logProducerConsumerAction(pProducer->sharedBufferName, PRODUCER_ROLE, index);

			pProducer ->writtenMessage++;
			// wake up consumers
			wakeup(pProducer->sharedBufferName, CONSUMER_ROLE);
			pProducer->indexWrite++;
			return;
		}
	}
}