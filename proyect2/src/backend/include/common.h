#ifndef COMMON_FILE
#define COMMON_FILE

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SHARED_BUFFER_NAME "/tmp/sharedBuffer"

// sharedBuffer = 16, dataMessage = 264, bufferElement = 304, assumming 10 elements in the buffer
#define STORAGE_SIZE 5696

#define MAX_BUFFER_ELEMENTS 100

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

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
	int producers;
	int consumers;
	sem_t producersMutex;
	sem_t consumersMutex;
} productConsumer;


typedef struct
{
	int id;
	int size;
	char name[50];
	productConsumer counter;
	bufferElement bufferElements[MAX_BUFFER_ELEMENTS];
} sharedBuffer;


typedef enum
{
	PRODUCER_ROLE = 0,
	CONSUMER_ROLE = 1,
	NONE_ROLE
} productConsumerRole;


typedef enum
{
	DECREASE= 0,
	INCREASE = 1
} productConsumerAction;


// Concatenates a char base name with an integer
char *getBufferName(char *baseName, int id);

double randomExponentialDistribution(double lambda);

int getCounter(productConsumer counter, productConsumerRole role);

void doActionToCounter(productConsumer counter, productConsumerRole role, productConsumerAction action);

#endif // COMMON_FILE