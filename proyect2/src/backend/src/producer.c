#include "include/producer.h"

#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <semaphore.h>


int writeData(bufferElement buffElement, dataMessage message)
{
	int semValue;
    if ((buffElement.indexAvailable != 1) || (sem_getvalue(&buffElement.mutex, &semValue) < 0) || (semValue == 0))
    {
    	printf("Warning, not possible to write in the shared buffer.\n");
    	return 0;
    }

    sem_wait(&buffElement.mutex);

    //critical section
    buffElement.data.producerId = message.producerId;
    strcpy(buffElement.data.date, message.date);
    buffElement.data.key = message.key;

    //signal
    sem_post(&buffElement.mutex);

    return 1;
}


void tryWrite(dataMessage message, char *bufferName)
{
	int fileDescriptor = open(bufferName, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fileDescriptor == -1)
	{
		printf("Error, open() failed, error code %d.\n", errno);
		return;
	}

	// map shared memory to process address space
	sharedBuffer *pSharedBuffer = (sharedBuffer*)mmap(NULL, STORAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
	if (pSharedBuffer == MAP_FAILED)
	{
		printf("Error, mmap() failed, error code %d.\n", errno);
		return;
	}

	for (int i = 0; i < pSharedBuffer->size; i++)
	{
		if (!writeData(pSharedBuffer->bufferElements[i], message))
		{
			continue;
		}
		else
		{
			return;
		}
	}
}