#include "include/common.h"

#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>


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


void doLogging(char *text, producerConsumerCounter counter)
{
	sem_wait(&counter.loggingFileMutex);

	FILE *fp = fopen(counter.loggingFileName, "a");
	if (fp == NULL)
	{
		printf("Error, fopen() failed.\n");
		return;
	}

	fputs(text, fp);
	fputs("\n", fp);

	fclose(fp);

	sem_post(&counter.loggingFileMutex);
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