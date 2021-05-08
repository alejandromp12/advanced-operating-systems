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



void logConsumerTermination(consumerProcess *pConsumer, int key)
{
	printf("==================================\n");
	printf("Consumer with pid %i is dead by read message with key %i ==> pid %i % 5 = key %i\n", pConsumer ->pid, key, pConsumer ->pid, key);
	printf("Consumer read %i messages from the mailbox\n", pConsumer ->readMessage);
	printf("==================================\n");
}

void xkillConsumer(consumerProcess *pConsumer)
{
	removeProducerConsumer(CONSUMER_ROLE, pConsumer ->sharedBufferName);
	removeProducerConsumerPIDFromList(pConsumer ->sharedBufferName, pConsumer ->pid, CONSUMER_ROLE);
	exit(0);
}



int readData(bufferElement *pBuffElement, int bufferIndex, char *bufferName, consumerProcess *pConsumer)
{
	int semValue;
    if ((pBuffElement->indexAvailable != 0) || (sem_getvalue(&(pBuffElement->mutex), &semValue) < 0) || (semValue == 0))
    {
		printf("Warning, not possible to read in the shared buffer.\n");
		return 0;
    }

	int killConsumer;
	int key;
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
	
	killConsumer = pBuffElement ->data.key == (pConsumer ->pid % 5) ? KILL_CONSUMER: NO_KILL_CONSUMER;
	key = pBuffElement ->data.key;
    //signal
    sem_post(&(pBuffElement->mutex));

	pConsumer ->readMessage++;

	if(killConsumer == KILL_CONSUMER)
	{
		logConsumerTermination(pConsumer, key);
		return KILL_CONSUMER;
	}

    return 1;
}


void tryRead(consumerProcess *pConsumer)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(pConsumer->sharedBufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return;
	}

	int index;
	int bufferSize = pSharedBuffer->size;
	for (pConsumer->readIndex; (pConsumer->readIndex % bufferSize) < bufferSize; pConsumer->readIndex++)
	{
		index = pConsumer->readIndex % bufferSize;
		int readDataReturn = readData(&(pSharedBuffer->bufferElements[index]), index, pConsumer->sharedBufferName, pConsumer);
		
		if (!readDataReturn)
		{
			if (isBufferEmpty(pSharedBuffer))
			{
				setProducerConsumerPIDState(pConsumer->sharedBufferName, pConsumer->pid, INACTIVE, CONSUMER_ROLE);
				doProcess(pConsumer->pid, STOP);
				printf("CONSUMER with PID %i, just woke up.\n", pConsumer->pid);
			}

			continue;
		}
		else
		{
			logProducerConsumerAction(pConsumer->sharedBufferName, CONSUMER_ROLE, index);

			if(readDataReturn == KILL_CONSUMER) xkillConsumer(pConsumer);
			// wake up consumers
			wakeup(pConsumer->sharedBufferName, PRODUCER_ROLE);
			pConsumer->readIndex++; 

			return;
		}
	}
}