#include "include/finisher.h"
#include "include/common.h"
#include "include/scheduler.h"
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <time.h>


int writeDataTerminateMessage(bufferElement *pBuffElement, dataMessage message, int bufferIndex, char *bufferName)
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
	
	pBuffElement->data.killFlag = message.killFlag;

    // no longer available
    pBuffElement->indexAvailable = 0;

	logTerminatorAction(bufferName, bufferIndex, pBuffElement, WRITE_MSG_FOR_CONSUMERS);

    //signal
    sem_post(&(pBuffElement->mutex));

    return 1;
}

int tryWriteTerminateMessage(dataMessage message, sharedBuffer *pSharedBuffer, int index, int terminatorPid)
{
	if (pSharedBuffer == NULL)
	{
		printf("Error, pSharedBuffer is NULL.\n");
		return -1;
	}

	int bufferSize = pSharedBuffer->size;
	for (index; index % bufferSize < bufferSize; index++)
	{	
		index = index % bufferSize;
		if (!writeDataTerminateMessage(&(pSharedBuffer->bufferElements[index]), message, index, pSharedBuffer->name))
		{
			if (isBufferFull(pSharedBuffer))
			{
				return 0;
			}

			continue;
		}
		else
		{
			// perform logging of the process
			printf("Terminator with PID %i, left a message for consumer with PID %i.\n", terminatorPid, message.killFlag);

			// wake up consumers to they read the message written by terminator
			//wakeup(bufferName, CONSUMER_ROLE);

			index++;
			return 1;
		}
	}

	return 0;
}

// Removes a buffer
int removeBuffer(char *bufferName)
{
	// map shared memory to process address space
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return 0;
	}

    if (munmap(pSharedBuffer, STORAGE_SIZE) != 0)
    {
		printf("Error, munmap() failed, error code %d.\n", errno);
		return 0;
    }

    if (remove(bufferName) == -1)
	{
		printf("Error, remove() failed, error code %d.\n", errno);
		return 0;
	}

    return 1;
}


int killProducers(char *bufferName)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return 0;
	}

	if (pSharedBuffer->killFlag != TERMINATE_SYSTEM) //SYSTEM WILL END
	{
		logTerminatorAction(bufferName, 0, NULL, WRITE_MSG_FOR_PRODUCERS);
	}

	pSharedBuffer->killFlag = TERMINATE_SYSTEM;
	pSharedBuffer->killerPID = getpid();

	// wake up poducers to they read the message written by terminator
	wakeup2(pSharedBuffer, PRODUCER_ROLE);
	sleep(1);

	//for(int i= 0; i < MAX_PIDS; i++)
	//{
	//	printf("[%i][%i]\n", pSharedBuffer ->PIDs.producersPIDs[i][0], pSharedBuffer ->PIDs.producersPIDs[i][1]);
	//}

	return 1;
}

int killConsumers(char *bufferName)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return 0;
	}

	//for(int i= 0; i < MAX_PIDS; i++)
	//{
	//	printf("[%i][%i]\n", pSharedBuffer ->PIDs.consumersPIDs[i][0], pSharedBuffer ->PIDs.consumersPIDs[i][1]);
	//}

	int index = 0;
	int terminatorPid = getpid();
	int consumerPid = -1;

	dataMessage data;
	data.producerId = 0;
	data.key = 0;
	data.killerPID = terminatorPid;
	time_t rawTime;
	struct tm *infoTime;
	time(&rawTime);
	for (int i = 0; i < MAX_PIDS; i++)
	{
		consumerPid = pSharedBuffer->PIDs.consumersPIDs[i][0];
		if (NO_PID != consumerPid)
		{
			data.killFlag = consumerPid;
			time(&rawTime);

			infoTime = localtime(&rawTime);
			strftime(data.date, sizeof(data.date), "%x - %X", infoTime);
			if (tryWriteTerminateMessage(data, pSharedBuffer, &index, terminatorPid) == 0)
			{
				// wake up consumers to they read the message written by terminator
				wakeup2(pSharedBuffer, CONSUMER_ROLE);
				usleep(500000);
			}
		}
	}

	return 1;
}

void logTerminatorAction(char *bufferName, int bufferIndex, bufferElement *pBuffElement, int whatToLog)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return;
	}

	// perform logging of the process
	char log[500];
	switch (whatToLog)
	{
		case REMOVE_SHARED_BUFFER:
		{
			sprintf(log,
		    		"==================================\n"
		    		"TERMINATOR is about to free shared buffer %s memory\n"
		    		"Consumers alive: %d\n"
		    		"Producers alive: %d\n"
		    		"==================================",
					bufferName,
					getProducerConsumer(CONSUMER_ROLE, bufferName),
					getProducerConsumer(PRODUCER_ROLE, bufferName));
			break;
		}

		case WRITE_MSG_FOR_CONSUMERS:
		{
			sprintf(log,
		    		"==================================\n"
		    		"TERMINATOR wrote a message at index %d of the buffer %s.\n"
		    		"Message is for consumer with PID %i.\n"
		    		"Date of the message %s.\n"
		    		"Consumers alive: %d\n"
		    		"Producers alive: %d\n"
		    		"==================================",
					bufferIndex + 1,
					bufferName,
					pBuffElement->data.killFlag,
					pBuffElement->data.date,
					getProducerConsumer(CONSUMER_ROLE, bufferName),
					getProducerConsumer(PRODUCER_ROLE, bufferName));
			break;
		}

		case WRITE_MSG_FOR_PRODUCERS:
		{
			sprintf(log,
		    		"==================================\n"
		    		"TERMINATOR raised the kill flag {TERMINATE_SYSTEM} of the shared buffer %s\n"
		    		"Producers are going to be killed once read it.\n"
		    		"Consumers alive: %d\n"
		    		"Producers alive: %d\n"
		    		"==================================",
					bufferName,
					getProducerConsumer(CONSUMER_ROLE, bufferName),
					getProducerConsumer(PRODUCER_ROLE, bufferName));
			break;
		}

		default:
			break;
	}

	doLogging(log, pSharedBuffer);
	printf("%s\n", log);
}