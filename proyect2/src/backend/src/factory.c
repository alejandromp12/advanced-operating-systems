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
int populateSharedBuffer(int bufferSize, int bufferId)
{

}


// Creates the shared buffer
int createSharedBuffer(int bufferSize, int bufferId)
{
	char sharedBufferName[50];
	strcpy(sharedBufferName, getBufferName(SHARED_BUFFER_NAME, bufferId));
	int fileDescriptor = createFileDescriptor(sharedBufferName, sizeof(sharedBuffer));
	if (fileDescriptor == -1)
	{
		printf("Error, getting fileDescriptor.\n");
		return 0;
	}

	sharedBuffer *pSharedBuffer = (sharedBuffer*)mmap(NULL, sizeof(sharedBuffer), PROT_WRITE | PROT_READ, MAP_SHARED, fileDescriptor, 0);
	if (pSharedBuffer == MAP_FAILED)
	{
		printf("Error, mmap() failed, error code %d.\n", errno);
		return 0;
	}

	char childBufferName[50];
	strcpy(childBufferName, getBufferName(BUFFER_ELEMENT_NAME, bufferId));
	fileDescriptor = createFileDescriptor(childBufferName, sizeof(bufferElement));
	if (fileDescriptor == -1)
	{
		printf("Error, getting fileDescriptor.\n");
		return 0;
	}

	bufferElement *pBufferElements = (bufferElement*)mmap(NULL, sizeof(bufferElement), PROT_WRITE | PROT_READ, MAP_SHARED, fileDescriptor, 0);
	if (pBufferElements == MAP_FAILED)
	{
		printf("Error, mmap() failed, error code %d.\n", errno);
		return 0;
	}

	for (int i = 0; i < bufferSize; i++)
	{
		pBufferElements[i].indexAvailable = 1;
		if (sem_init(&(pBufferElements[i].mutex), 1, 1) < 0)
		{
			printf("Error, sem_init() failed at index: %d.\n", i);
			return 0;
		}
	}

	pSharedBuffer->size = bufferSize;
	pSharedBuffer->bufferId = bufferId;
	strcpy(pSharedBuffer->sharedBufferName, sharedBufferName);
	strcpy(pSharedBuffer->childBufferName, childBufferName);

	return 1;
}