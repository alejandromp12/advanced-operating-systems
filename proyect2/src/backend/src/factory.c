#include "include/factory.h"
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>


// Creates the shared buffer
int createSharedBuffer(int bufferSize, int bufferId)
{
	_pSharedBuffer = (sharedBuffer*)malloc(sizeof(sharedBuffer));
	if (_pSharedBuffer == NULL)
	{
		return 0;
	}

	_pSharedBuffer->pBufferElements = (bufferElement*)mmap(NULL, bufferSize * sizeof(bufferElement), PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
	if (_pSharedBuffer->pBufferElements == MAP_FAILED)
	{
		return 0;
	}

	for (int i = 0; i < bufferSize; i++)
	{
		(_pSharedBuffer->pBufferElements[i]).indexAvailable = 1;
	}

	_pSharedBuffer->size = bufferSize;
	_pSharedBuffer->bufferId = bufferId;

	return 1;
}