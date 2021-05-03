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
int getFileDescriptor(char *bufferName)
{
	int fileDescriptor = creat(bufferName, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fileDescriptor == -1)
	{
		printf("Error, creat() failed, error code %d.\n", errno);
		return -1;
	}

	if (ftruncate(fileDescriptor, STORAGE_SIZE) == -1)
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


// Creates the shared buffer
int createSharedBuffer(int bufferSize, char *bufferName)
{
	int fileDescriptor = getFileDescriptor(bufferName);
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

	bufferElement *pBufferElements = (bufferElement*)malloc(bufferSize * sizeof(bufferElement));
	if (pBufferElements == NULL)
	{
		printf("Error, pBufferElements is NULL.\n");
		return 0;
	}

	for (int i = 0; i < bufferSize; i++)
	{
		pBufferElements[i].indexAvailable = 1;

		if (sem_init(&pBufferElements[i].mutex, 1, 1) < 0)
		{
			printf("Error, sem_init() failed at index: %d.\n", i);
			return 0;
		}

		pSharedBuffer->bufferElements[i] = pBufferElements[i];
	}

	pSharedBuffer->size = bufferSize;
	strcpy(pSharedBuffer->bufferName, bufferName);

	return 1;
}