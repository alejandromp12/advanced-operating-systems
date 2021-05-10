#include "include/common.h"

#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>

void getLogFileData(sharedBuffer *pSharedBuffer)
{
	char line[255] = {0};
	int sizeLines = 0;
	if (pSharedBuffer == NULL)
	{
		printf("Error, pSharedBuffer is NULL.\n");
		return;
	}

	sem_wait(&pSharedBuffer->loggingFileMutex);

	FILE *fp = fopen(pSharedBuffer->loggingFileName, "r");
	if (fp == NULL)
	{
		printf("Error, fopen() failed.\n");
		sem_post(&pSharedBuffer->loggingFileMutex);
		return;
	}

	int numberLine = 0;
	while (fgets(line, 255, fp) != NULL)
	{
		if(NUMBER_LINE == numberLine)
		{
			strcat(LOGGING_BUFFER, line);
			NUMBER_LINE ++;
		}
		
		numberLine ++;
	}

	fclose(fp);

	sem_post(&pSharedBuffer->loggingFileMutex);
}

// Concatenates a char base name with an integer
char *getFixedName(char *baseName, int id)
{
	char bufferName[50];
	strcpy(bufferName, baseName);

	char intToString[5];
	sprintf(intToString, "%d", id);

	return strcat(bufferName, intToString);
}


// Returns the random exponential distribution of a lambda value
double getRandomExponentialDistribution(double lambda)
{
	double intermediate = 0.0;
	intermediate = rand() / (RAND_MAX + 1.0);
	return -log(1 - intermediate) / lambda;
}


void doLogging(char *text, sharedBuffer *pSharedBuffer)
{
	if (pSharedBuffer == NULL)
	{
		printf("Error, pSharedBuffer is NULL.\n");
	}

	sem_wait(&pSharedBuffer->loggingFileMutex);

	FILE *fp = fopen(pSharedBuffer->loggingFileName, "a");
	if (fp == NULL)
	{
		printf("Error, fopen() failed.\n");
		sem_post(&pSharedBuffer->loggingFileMutex);
		return;
	}

	fputs(text, fp);
	//fputs("\n", fp);

	fclose(fp);

	sem_post(&pSharedBuffer->loggingFileMutex);
}


sharedBuffer *getSharedBuffer(char *bufferName)
{
	int fileDescriptor = open(bufferName, O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fileDescriptor == -1)
	{
		printf("Error, open() failed, error code %d.\n", errno);
		return NULL;
	}

	// map shared memory to process address space
	sharedBuffer *pSharedBuffer = (sharedBuffer*)mmap(NULL, STORAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
	if (pSharedBuffer == MAP_FAILED)
	{
		printf("Error, mmap() failed, error code %d.\n", errno);
		return NULL;
	}

	return pSharedBuffer;
}


int bufferAvailableSpace(sharedBuffer *pSharedBuffer)
{
	int availableSpace = 0;
	for (int i = 0; i < pSharedBuffer->size; i++)
	{
    	sem_wait(&(pSharedBuffer->bufferElements[i].mutex));
    	if (pSharedBuffer->bufferElements[i].indexAvailable == 1)
    	{
    		++availableSpace;
    	}
    	sem_post(&(pSharedBuffer->bufferElements[i].mutex));
	}

    return availableSpace;
}


int isBufferFull(sharedBuffer *pSharedBuffer)
{
	return (0 == bufferAvailableSpace(pSharedBuffer));
}


int isBufferEmpty(sharedBuffer *pSharedBuffer)
{
	return (pSharedBuffer->size == bufferAvailableSpace(pSharedBuffer));
}