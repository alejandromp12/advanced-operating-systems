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

	pSharedBuffer ->killFlag = TERMINATE_SYSTEM; //SYSTEM WILL END
	// LOG  of this !!! 
	wakeup(bufferName, PRODUCER_ROLE);

	return 1;
}

int killConsumer(char *bufferName)
{

}



