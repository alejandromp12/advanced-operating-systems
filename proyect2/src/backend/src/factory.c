#include "include/factory.h"

#include "include/common.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

// Creates the shared buffer
int createSharedBuffer(int bufferSize, char *bufferName)
{
	int fileDescriptor = shm_open(bufferName, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
	if (fileDescriptor == -1)
	{
		printf("Error, opening fileDescriptor %s, error code %d.\n", bufferName, errno);
		return 0;
	}

	if (ftruncate(fileDescriptor, sizeof(sharedBuffer)))
	{
		printf("Error, trunkating fileDescriptor.\n");
		return 0;
	}

	sharedBuffer *pSharedBuffer = (sharedBuffer*)mmap(NULL, sizeof(sharedBuffer), PROT_READ|PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
	if (pSharedBuffer == MAP_FAILED)
	{
		printf("Error, mmaping.\n");
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
		pSharedBuffer->bufferElements[i] = pBufferElements[i];
	}


	pSharedBuffer->size = bufferSize;
	strcpy(pSharedBuffer->bufferName, bufferName);

	return 1;
}