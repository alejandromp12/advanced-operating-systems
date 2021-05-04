#include "include/finisher.h"

#include "include/common.h"
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
	int fileDescriptor = open(bufferName, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fileDescriptor == -1)
	{
		printf("Error, open() failed, error code %d.\n", errno);
		return 0;
	}

	// map shared memory to process address space
	sharedBuffer *pSharedBuffer = (sharedBuffer*)mmap(NULL, sizeof(sharedBuffer), PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
	if (pSharedBuffer == MAP_FAILED)
	{
		printf("Error, mmap() failed, error code %d.\n", errno);
		return 0;
	}

	fileDescriptor = open(pSharedBuffer->childBufferName, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fileDescriptor == -1)
	{
		printf("Error, open() failed, error code %d.\n", errno);
		return 0;
	}

	// map shared memory to process address space
	bufferElement *pChildBuffer = (bufferElement*)mmap(NULL, sizeof(bufferElement), PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
	if (pChildBuffer == MAP_FAILED)
	{
		printf("Error, mmap() failed, error code %d.\n", errno);
		return 0;
	}

    if (munmap(pChildBuffer, sizeof(bufferElement)) != 0)
    {
		printf("Error, munmap() failed, error code %d.\n", errno);
		return 0;
    }

    if (remove(pSharedBuffer->childBufferName) == -1)
	{
		printf("Error, remove() failed, error code %d.\n", errno);
		return 0;
	}

    if (munmap(pSharedBuffer, sizeof(sharedBuffer)) != 0)
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