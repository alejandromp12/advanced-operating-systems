#include "include/producer.h"

#include "include/common.h"
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int writeData(bufferElement buffElement, dataMessage message)
{
	int semValue;
    //wait
    if ((buffElement.indexAvailable != 1) || (sem_getvalue(&buffElement.mutex, &semValue) < 0) || (semValue == 0))
    {
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
	int fileDescriptor = shm_open(bufferName, O_RDWR, S_IRUSR|S_IWUSR);
	if (fileDescriptor == -1)
	{
		printf("Error, shm_open.\n");
		return;
	}

	// map shared memory to process address space
	sharedBuffer *pSharedBuffer = (sharedBuffer*)mmap(NULL, sizeof(sharedBuffer), PROT_READ|PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
	if (pSharedBuffer == MAP_FAILED)
	{
		printf("Error, mmap.\n");
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