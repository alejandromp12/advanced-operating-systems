#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "backend/include/consumer.h"
#include "backend/include/factory.h"
#include "backend/include/finisher.h"
#include "backend/include/producer.h"
#include "frontend/include/gui.h"


int main(int argc, char  *argv[])
{
	// simple test, just for review functionality
	int bufferSize = 3;
	if (!createSharedBuffer(bufferSize, 0))
	{
		printf("Error, creating shared buffer.\n");
		return 1;
	}

	for (int i = 0; i < bufferSize; i++)
	{
		printf("Buffer id %i is %s.\n", i, (_pSharedBuffer->pBufferElements[i]).indexAvailable ? "available" : "not available");
	}

    if (munmap(_pSharedBuffer->pBufferElements, bufferSize * sizeof(bufferElement)) != 0)
    {
		printf("UnMapping Failed\n");
		return 1;
    }
    // ends

	return 0;
}
