#ifndef COMMON_FILE
#define COMMON_FILE

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

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
	int id;
	int size;
	char name[50];
	bufferElement bufferElements[MAX_BUFFER_ELEMENTS];
} sharedBuffer;

// Concatenates a char base name with an integer
char *getBufferName(char *baseName, int id);

#endif // COMMON_FILE