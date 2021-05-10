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
#include <sys/time.h>

int writeData(bufferElement *pBuffElement, producerProcess *pProducer, dataMessage message, int bufferIndex, char *bufferName)
{
	int semValue;
    if ((pBuffElement->indexAvailable != 1) || (sem_getvalue(&(pBuffElement->mutex), &semValue) < 0) || (semValue == 0))
    {
    	printf("Warning, not possible to write in the shared buffer.\n");
    	return 0;
    }

    struct timeval startTime;
	gettimeofday(&startTime, NULL);

    sem_wait(&(pBuffElement->mutex));

    struct timeval endTime;
	gettimeofday(&endTime, NULL);
    double delta = (endTime.tv_sec - startTime.tv_sec) * 1e6;
    delta = (delta + (endTime.tv_usec - startTime.tv_usec)) * 1e-6;

    pProducer->timeSpendInMutex += delta;

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
	printf("Consumers alive: %d\n", getProducerConsumer(CONSUMER_ROLE, bufferName));
	printf("Producers alive: %d\n", getProducerConsumer(PRODUCER_ROLE, bufferName));
	printf("==================================\n");

    //signal
    sem_post(&(pBuffElement->mutex));

    return 1;
}

void logProducerStatistics(producerProcess *pProducer, sharedBuffer *pSharedBuffer)
{
	if ((pProducer == NULL) || (pSharedBuffer == NULL))
	{
		printf("Error, in logProducerStatistics(), a ptr is NULL.\n");
		return;
	}
 
	struct timeval endTime;
	gettimeofday(&endTime, NULL);

	double producerProgramElapsedTime = (endTime.tv_sec - pProducer->startTime.tv_sec) * 1e6;
	producerProgramElapsedTime = (producerProgramElapsedTime + (endTime.tv_usec - pProducer->startTime.tv_usec)) * 1e-6;
	// perform logging of the process
	char log[250];
	sprintf(log,
		    "==================================\n"
		    "PRODUCER statistics\n"
		    "ID: %d\n"
		    "PID: %d\n"
		    "Messages written: %d\n"
		    "Time blocked by semaphores: %f s\n"
		    "Active time: %f s\n"
		    "Idle time: %f s\n"
		    "==================================",
			pProducer->producerId,
			pProducer->pid,
			pProducer->writtenMessages,
			pProducer->timeSpendInMutex,
			producerProgramElapsedTime,
			pProducer->idleTime);

	doLogging(log, pSharedBuffer);

	//printf("Message logged:\n%s\n", log);
}


void xkillProducer(producerProcess *pProducer, sharedBuffer *pSharedBuffer)
{
	removeProducerConsumer(PRODUCER_ROLE, pProducer->sharedBufferName);
	removeProducerConsumerPIDFromList(pProducer->sharedBufferName, pProducer->pid, PRODUCER_ROLE);

	printf("==================================\n");
	printf("Producer with pid %i killed due to termination flag\n", pProducer->pid);
	printf("Producer put %i messages in the mailbox\n", pProducer->writtenMessages);
	printf("==================================\n");

	logProducerStatistics(pProducer, pSharedBuffer);

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

	if (pSharedBuffer->killFlag == TERMINATE_SYSTEM)
	{
		xkillProducer(pProducer, pSharedBuffer);
	}

	int index;
	int bufferSize = pSharedBuffer->size;
	for (pProducer->indexWrite; (pProducer->indexWrite % bufferSize) < bufferSize; pProducer->indexWrite++)
	{
		index = pProducer->indexWrite % bufferSize;
		if (!writeData(&(pSharedBuffer->bufferElements[index]), pProducer, message, index, pProducer->sharedBufferName))
		{	
			if (pSharedBuffer->killFlag == TERMINATE_SYSTEM) 
			{
				xkillProducer(pProducer, pSharedBuffer);
			}

			if (isBufferFull(pSharedBuffer))
			{
				// ===================================
				// ===================================
				// CRITICAL SECTION DO NOT CHANGE THIS
				wakeup(pProducer->sharedBufferName, CONSUMER_ROLE);

    			struct timeval startTime;
				gettimeofday(&startTime, NULL);

				setProducerConsumerPIDState(pSharedBuffer, pProducer->pid, INACTIVE, PRODUCER_ROLE);
				doProcess(pProducer->pid, STOP);

    			struct timeval endTime;
				gettimeofday(&endTime, NULL);
    			double delta = (endTime.tv_sec - startTime.tv_sec) * 1e6;
    			delta = (delta + (endTime.tv_usec - startTime.tv_usec)) * 1e-6;
    			pProducer->idleTime += delta;

				printf("PRODUCER with PID %i, just woke up.\n", pProducer->pid);
				// ===================================
				// ===================================
				// ===================================
			}

			continue;

		}
		else
		{
			// perform logging of the process
			logProducerConsumerAction(pProducer->sharedBufferName, PRODUCER_ROLE, index);

			pProducer->writtenMessages++;
			// wake up consumers
			wakeup(pProducer->sharedBufferName, CONSUMER_ROLE);
			pProducer->indexWrite++;
			return;
		}
	}
}