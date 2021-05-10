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
#include <sys/time.h>

void logConsumerTerminationByMod5(consumerProcess *pConsumer, int key)
{
	printf("==================================\n");
	printf("Consumer with pid %i is dead by read message with key %i ==> pid %i MOD 5 = key %i\n", pConsumer->pid, key, pConsumer->pid, key);
	printf("Consumer read %i messages from the mailbox\n", pConsumer->readMessages);
	printf("==================================\n");
}

void logConsumerTerminationByTerminator(consumerProcess *pConsumer)
{
	printf("==================================\n");
	printf("Consumer with pid %i killed due to termination message", pConsumer->pid);
	printf("Consumer read %i messages from the mailbox\n", pConsumer->readMessages);
	printf("==================================\n");
}

void logConsumerStatistics(consumerProcess *pConsumer, sharedBuffer *pSharedBuffer)
{
	if ((pConsumer == NULL) || (pSharedBuffer == NULL))
	{
		printf("Error, in logProducerStatistics(), a ptr is NULL.\n");
		return;
	}
 
	struct timeval endTime;
	gettimeofday(&endTime, NULL);

	double consumerProgramElapsedTime = (endTime.tv_sec - pConsumer->startTime.tv_sec) * 1e6;
	consumerProgramElapsedTime = (consumerProgramElapsedTime + (endTime.tv_usec - pConsumer->startTime.tv_usec)) * 1e-6;
	// perform logging of the process
	char log[250];
	sprintf(log,
		    "==================================\n"
		    "CUNSUMER statistics\n"
		    "ID: %d\n"
		    "PID: %d\n"
		    "Messages read: %d\n"
		    "Time blocked by semaphores: %f s\n"
		    "Active time: %f s\n"
		    "Idle time: %f s\n"
		    "==================================",
			pConsumer->consumerId,
			pConsumer->pid,
			pConsumer->readMessages,
			pConsumer->timeSpendInMutex,
			consumerProgramElapsedTime,
			pConsumer->idleTime);

	doLogging(log, pSharedBuffer);

	//printf("Message logged:\n%s\n", log);
}

void xkillConsumer(consumerProcess *pConsumer, sharedBuffer *pSharedBuffer)
{
	removeProducerConsumer(CONSUMER_ROLE, pConsumer->sharedBufferName);
	removeProducerConsumerPIDFromList(pConsumer->sharedBufferName, pConsumer->pid, CONSUMER_ROLE);

	logConsumerStatistics(pConsumer, pSharedBuffer);
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
	int killConsumerByTerminator;
	int key;

    struct timeval startTime;
	gettimeofday(&startTime, NULL);

    sem_wait(&(pBuffElement->mutex));

    struct timeval endTime;
	gettimeofday(&endTime, NULL);
    double delta = (endTime.tv_sec - startTime.tv_sec) * 1e6;
    delta = (delta + (endTime.tv_usec - startTime.tv_usec)) * 1e-6;

    pConsumer->timeSpendInMutex += delta;

	if(pBuffElement->data.killFlag != NO_PID && pBuffElement->data.killFlag != pConsumer->pid)
	{
		printf("Warning, not possible to read in the shared buffer because message is for another consumer (Dead message).\n");
		sem_post(&(pBuffElement->mutex));
		return 0;
	}


    //critical section
    printf("==================================\n");
	printf("Message READ at index %d of the buffer %s.\n", bufferIndex + 1, bufferName);
	printf("Producer ID who wrote the message %i.\n", pBuffElement->data.producerId);
	printf("Key of the message %i.\n", pBuffElement->data.key);
	printf("Date of the message %s.\n", pBuffElement->data.date);
	printf("Consumers alive: %d\n", getProducerConsumer(CONSUMER_ROLE, bufferName));
	printf("Producers alive: %d\n", getProducerConsumer(PRODUCER_ROLE, bufferName));
	printf("==================================\n");


	//Cleaning?
	//pBuffElement->data.producerId = message.producerId;
    //strcpy(pBuffElement->data.date, message.date);
    //pBuffElement->data.key = message.key;

    // now available
    pBuffElement->indexAvailable = 1;
	
	killConsumer = pBuffElement->data.key == (pConsumer->pid % 5) ? KILL_CONSUMER: NO_KILL_CONSUMER;
	key = pBuffElement->data.key;
	killConsumerByTerminator = pBuffElement->data.killFlag;

	if(pBuffElement->data.killerPID != NO_PID)
	{
		pConsumer->killerPID = pBuffElement->data.killerPID;
	}

	dataMessage tmp;
	strcpy(tmp.date,"");
	tmp.key = NO_PID;
	tmp.producerId = NO_PID;

	pBuffElement->data = tmp;
    //signal
    sem_post(&(pBuffElement->mutex));

	pConsumer->readMessages++;

	if (killConsumerByTerminator == pConsumer->pid)
	{
		return KILL_CONSUMER_BY_TERMINATOR;
	}
	else if (killConsumer == KILL_CONSUMER)
	{
		logConsumerTerminationByMod5(pConsumer, key);
		return KILL_CONSUMER;
	}
	else
	{
    	return 1;
	}
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
		if (readDataReturn == 0)
		{
			if (isBufferEmpty(pSharedBuffer))
			{	
				if (pSharedBuffer->killerPID != NO_PID)
				{
					doProcess(pSharedBuffer->killerPID, CONTINUE);
				}

				wakeup(pConsumer->sharedBufferName, PRODUCER_ROLE);
				setProducerConsumerPIDState(pSharedBuffer, pConsumer->pid, INACTIVE, CONSUMER_ROLE);

    			struct timeval startTime;
				gettimeofday(&startTime, NULL);

				doProcess(pConsumer->pid, STOP);

    			struct timeval endTime;
				gettimeofday(&endTime, NULL);
    			double delta = (endTime.tv_sec - startTime.tv_sec) * 1e6;
    			delta = (delta + (endTime.tv_usec - startTime.tv_usec)) * 1e-6;
    			pConsumer->idleTime += delta;

				printf("CONSUMER with PID %i, just woke up.\n", pConsumer->pid);
			}

			continue;
		}
		else if (readDataReturn == KILL_CONSUMER_BY_TERMINATOR) 
		{
			//doProcess(pConsumer ->killerPID, CONTINUE);
			logProducerConsumerAction(pConsumer->sharedBufferName, CONSUMER_ROLE, index);
			logConsumerTerminationByTerminator(pConsumer);

			wakeup(pConsumer->sharedBufferName, PRODUCER_ROLE);

			xkillConsumer(pConsumer, pSharedBuffer);
		}
		else if(readDataReturn == KILL_CONSUMER)
		{
			logProducerConsumerAction(pConsumer->sharedBufferName, CONSUMER_ROLE, index);

			wakeup(pConsumer->sharedBufferName, PRODUCER_ROLE);

			xkillConsumer(pConsumer, pSharedBuffer);
		}
		else
		{
			logProducerConsumerAction(pConsumer->sharedBufferName, CONSUMER_ROLE, index);
			// wake up producer
			wakeup(pConsumer->sharedBufferName, PRODUCER_ROLE);
			pConsumer->readIndex++; 
			return;
		}
	}
}