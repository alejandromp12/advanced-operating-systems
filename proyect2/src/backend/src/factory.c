#include "include/factory.h"

#include "include/common.h"
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>

// populates matrix
void populatePIDsList(int pidsList[MAX_PIDS][MAX_STATES])					
{
	for (int i = 0; i < MAX_PIDS; i++)
	{
		pidsList[i][0] = NO_PID;
		pidsList[i][1] = INACTIVE;
		
	}
}


// Inits producer-consumer lists
int initializeProducersConsumersPIDs(sharedBuffer *pSharedBuffer)
{
	if (sem_init(&(pSharedBuffer->PIDs.producersPIDsMutex), 1, 1) < 0)
	{
		printf("Error, sem_init() failed.\n");
		return 0;
	}

	if (sem_init(&(pSharedBuffer->PIDs.consumersPIDsMutex), 1, 1) < 0)
	{
		printf("Error, sem_init() failed.\n");
		return 0;
	}

	populatePIDsList(pSharedBuffer->PIDs.producersPIDs);
	populatePIDsList(pSharedBuffer->PIDs.consumersPIDs);

	return 1;
}


// Inits counter struct
int initializeCounter(sharedBuffer *pSharedBuffer, int bufferId)
{
	pSharedBuffer->counter.producers = 0;
	pSharedBuffer->counter.consumers = 0;

	if (sem_init(&(pSharedBuffer->counter.producersMutex), 1, 1) < 0)
	{
		printf("Error, sem_init() failed.\n");
		return 0;
	}

	if (sem_init(&(pSharedBuffer->counter.consumersMutex), 1, 1) < 0)
	{
		printf("Error, sem_init() failed.\n");
		return 0;
	}

	if (sem_init(&(pSharedBuffer->counter.loggingFileMutex), 1, 1) < 0)
	{
		printf("Error, sem_init() failed.\n");
		return 0;
	}

	strcpy(pSharedBuffer->counter.loggingFileName, getFixedName(LOGGING_FILE_NAME, bufferId));

	FILE *fp = fopen(pSharedBuffer->counter.loggingFileName, "w+");
	if (fp == NULL)
	{
		printf("Error, fopen() failed.\n");
		return 0;
	}

	fclose(fp);

	return 1;
}


// Creates file and returns its descriptor
int createFileDescriptor(char *bufferName, int size)
{
	int fileDescriptor = creat(bufferName, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fileDescriptor == -1)
	{
		printf("Error, creat() failed, error code %d.\n", errno);
		return -1;
	}

	if (ftruncate(fileDescriptor, size) == -1)
	{
		printf("Error, ftruncate() failed, error code %d.\n", errno);
		return -1;
	}

	fileDescriptor = openat(fileDescriptor, bufferName, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fileDescriptor == -1)
	{
		printf("Error, openat() failed, error code %d.\n", errno);
		return -1;
	}

	return fileDescriptor;
}


// Populates the shared buffer
int populateSharedBuffer(int bufferSize, int bufferId, char *sharedBufferName, sharedBuffer *pSharedBuffer)
{
	for (int i = 0; i < bufferSize; i++)
	{
		pSharedBuffer->bufferElements[i].indexAvailable = 1;
		if (sem_init(&(pSharedBuffer->bufferElements[i].mutex), 1, 1) < 0)
		{
			printf("Error, sem_init() failed at index: %d.\n", i);
			return 0;
		}
	}

	pSharedBuffer->size = bufferSize;
	pSharedBuffer->id = bufferId;
	strcpy(pSharedBuffer->name, sharedBufferName);

	// initialize counter
	if (!initializeCounter(pSharedBuffer, bufferId))
	{
		printf("Error, initializeCounter() failed.\n");
		return 0;
	}

	// initialize pids list
	if (!initializeProducersConsumersPIDs(pSharedBuffer))
	{
		printf("Error, initializeProducersConsumersPIDs() failed.\n");
		return 0;
	}

	return 1;
}


// Creates the shared buffer
int createSharedBuffer(int bufferSize, int bufferId)
{
	char sharedBufferName[50];
	strcpy(sharedBufferName, getFixedName(SHARED_BUFFER_NAME, bufferId)); 
	int fileDescriptor = createFileDescriptor(sharedBufferName, STORAGE_SIZE);
	if (fileDescriptor == -1)
	{
		printf("Error, getting fileDescriptor.\n");
		return 0;
	}

	sharedBuffer *pSharedBuffer = (sharedBuffer*)mmap(NULL, STORAGE_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, fileDescriptor, 0);
	if (pSharedBuffer == MAP_FAILED)
	{
		printf("Error, mmap() failed, error code %d.\n", errno);
		return 0;
	}

	if (!populateSharedBuffer(bufferSize, bufferId, sharedBufferName, pSharedBuffer))
	{
		printf("Error, populateSharedBuffer() failed.\n");
		return 0;
	}

	return 1;
}