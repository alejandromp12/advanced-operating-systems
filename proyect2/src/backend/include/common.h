#ifndef COMMON_FILE
#define COMMON_FILE

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SHARED_BUFFER_NAME "/tmp/sharedBuffer"

// sharedBuffer = 16, dataMessage = 264, bufferElement = 304, assumming 10 elements in the buffer
#define STORAGE_SIZE 5696 // 5.5625MB

#define MAX_BUFFER_ELEMENTS 100

#define LOGGING_FILE_NAME "/tmp/sharedBuffer.log."
#define LOGGING_FILE_SIZE 5120 // 5MB

typedef struct
{
	int producerId;
	char date[80];
	int key;
} dataMessage;


typedef struct
{
	sem_t mutex;
	dataMessage data;
	int indexAvailable;
} bufferElement;


typedef struct
{
	char loggingFileName[50];
	int producers;
	int consumers;
	sem_t producersMutex;
	sem_t consumersMutex;
	sem_t loggingFileMutex;
} productConsumer;


typedef struct
{
	int id;
	int size;
	char name[50];
	productConsumer counter;
	bufferElement bufferElements[MAX_BUFFER_ELEMENTS];
} sharedBuffer;


// Concatenates a char base name with an integer
char *getFixedName(char *baseName, int id);

double getRandomExponentialDistribution(double lambda);

void doLogging(char *text, productConsumer counter);

void sleepProcess(char *bufferName, int pid);

sharedBuffer *getSharedBuffer(char *bufferName);

#endif // COMMON_FILE