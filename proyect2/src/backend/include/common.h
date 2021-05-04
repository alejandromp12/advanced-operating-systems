#ifndef COMMON_FILE
#define COMMON_FILE

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define SHARED_BUFFER_NAME "/tmp/sharedBuffer"
#define BUFFER_ELEMENT_NAME "/tmp/bufferElement"
// sharedBuffer = 16, dataMessage = 264, bufferElement = 304, assumming 10 elements in the buffer
#define STORAGE_SIZE 5696

typedef struct
{
	int producerId;
	char date[80];
	int key;
} dataMessage;


typedef struct
{
	sem_t mutex;
	int producerId;
	char date[80];
	int key;
	int indexAvailable;
} bufferElement;


typedef struct
{
	int bufferId;
	int size;
	char sharedBufferName[50];
	char childBufferName[50];
} sharedBuffer;


// Concatenates a char base name with an integer
char *getBufferName(char *baseName, int id);

#endif // COMMON_FILE