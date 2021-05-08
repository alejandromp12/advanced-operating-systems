#ifndef COMMON_FILE
#define COMMON_FILE

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SHARED_BUFFER_NAME "/tmp/sharedBuffer"

// sharedBuffer = 16, dataMessage = 264, bufferElement = 304, assumming 10 elements in the buffer
#define STORAGE_SIZE 102400 // 100MB

#define MAX_BUFFER_ELEMENTS 100
#define MAX_PIDS 50
#define MAX_STATES MAX_PIDS
#define NO_PID -1

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
} producerConsumerCounter;

typedef struct
{
	sem_t producersPIDsMutex;
	sem_t consumersPIDsMutex;
	int producersPIDs[MAX_PIDS][MAX_STATES];
	int consumersPIDs[MAX_PIDS][MAX_STATES];
} producerConsumePIDs;

typedef struct
{
	int id;
	int size;
	char name[50];
	producerConsumerCounter counter;
	producerConsumePIDs PIDs;
	bufferElement bufferElements[MAX_BUFFER_ELEMENTS];
} sharedBuffer;

typedef enum
{
	INACTIVE= 0,
	ACTIVE = 1
} producerConsumerPidState;

// Concatenates a char base name with an integer
char *getFixedName(char *baseName, int id);

double getRandomExponentialDistribution(double lambda);

void doLogging(char *text, producerConsumerCounter counter);

sharedBuffer *getSharedBuffer(char *bufferName);

#endif // COMMON_FILE