#include "include/factory.h"

#include "include/common.h"
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>


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

	return 1;
}


// Creates the shared buffer
int createSharedBuffer(int bufferSize, int bufferId)
{
	char sharedBufferName[50];
	strcpy(sharedBufferName, getBufferName(SHARED_BUFFER_NAME, bufferId)); 
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