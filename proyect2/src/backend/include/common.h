#ifndef COMMON_FILE
#define COMMON_FILE

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define TERMINATE_SYSTEM 0
#define START_SYSTEM 1
#define KILL_CONSUMER 20
#define KILL_CONSUMER_BY_TERMINATOR 30
#define NO_KILL_CONSUMER 10

#define SHARED_BUFFER_NAME "/tmp/sharedBuffer"

// sharedBuffer = 16, dataMessage = 264, bufferElement = 304, assumming 10 elements in the buffer
#define STORAGE_SIZE 102400 // 100MB

#define MAX_BUFFER_ELEMENTS 100
#define MAX_PIDS 50
#define MAX_STATES 2
#define NO_PID -1

#define LOGGING_FILE_NAME "/tmp/sharedBuffer.log."
#define LOGGING_FILE_SIZE 5120 // 5MB

typedef struct
{
	int producerId;
	char date[80];
	int key;
	int killFlag;
	int killerPID;
} dataMessage;

typedef struct
{
	sem_t mutex;
	dataMessage data;
	int indexAvailable;

} bufferElement;

typedef struct
{
	int producers;
	int consumers;
	sem_t producersMutex;
	sem_t consumersMutex;
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
	char loggingFileName[50];
	sem_t loggingFileMutex;
	producerConsumerCounter counter;
	producerConsumePIDs PIDs;
	bufferElement bufferElements[MAX_BUFFER_ELEMENTS];
	int killFlag;
	int killerPID;
} sharedBuffer;

typedef enum
{
	INACTIVE= 0,
	ACTIVE = 1
} producerConsumerPidState;

// Concatenates a char base name with an integer
char *getFixedName(char *baseName, int id);

double getRandomExponentialDistribution(double lambda);

void doLogging(char *text, sharedBuffer *pSharedBuffer);

sharedBuffer *getSharedBuffer(char *bufferName);

int isBufferFull(sharedBuffer *pSharedBuffer);

int isBufferEmpty(sharedBuffer *pSharedBuffer);

#endif // COMMON_FILE