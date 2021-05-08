#include "include/finisher.h"
#include "include/common.h"
#include "include/producerConsumerManager.h"
#include "include/scheduler.h"
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>


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


int killProducer(char *bufferName)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return 0;
	}

	sem_wait(&(pSharedBuffer->PIDs.producersPIDsMutex));

	int pid;
	for (int i = 0; i < MAX_PIDS; i++)
	{
		if (pSharedBuffer->PIDs.producersPIDs[i][0] != NO_PID)
		{
			pid = pSharedBuffer->PIDs.producersPIDs[i][0];
			doProcess(pid, KILL);
			removeProducerConsumerPIDFromList(bufferName, pid, PRODUCER_ROLE);
		}
	}

	sem_post(&(pSharedBuffer->PIDs.producersPIDsMutex));

	return 1;
}

int killConsumer(char *bufferName)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return 0;
	}

	sem_wait(&(pSharedBuffer->PIDs.consumersPIDsMutex));

	int pid;
	for (int i = 0; i < MAX_PIDS; i++)
	{
		if (pSharedBuffer->PIDs.consumersPIDs[i][0] != NO_PID)
		{
			pid = pSharedBuffer->PIDs.consumersPIDs[i][0];
			doProcess(pid, KILL);
			removeProducerConsumerPIDFromList(bufferName, pid, CONSUMER_ROLE);
		}
	}

	sem_post(&(pSharedBuffer->PIDs.consumersPIDsMutex));

	return 1;
}



