#include "include/finisher.h"

#include "include/common.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


// Removes a buffer
int removeBuffer(char *bufferName)
{
	int fileDescriptor = shm_open(bufferName, O_RDWR, S_IRUSR|S_IWUSR);
	if (fileDescriptor == -1)
	{
		printf("Error, shm_open.\n");
		return 0;
	}

	// map shared memory to process address space
	sharedBuffer *pSharedBuffer = (sharedBuffer*)mmap(NULL, sizeof(sharedBuffer), PROT_READ|PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
	if (pSharedBuffer == MAP_FAILED)
	{
		printf("Error, mmap.\n");
		return 0;
	}


    if (munmap(pSharedBuffer, sizeof(sharedBuffer)) != 0)
    {
		printf("UnMapping Failed\n");
		return 0;
    }

    return 1;
}