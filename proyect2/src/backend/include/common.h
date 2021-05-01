#ifndef COMMON_FILE
#define COMMON_FILE

#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#define STORAGE_ID "/tmp/sharedBuffer" // review ths location

typedef struct
{
	int producerId;
	char date[255]; // see https://linux.die.net/man/3/strftime
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
	int size;
	char *bufferName;
	bufferElement bufferElements[];
} sharedBuffer;

#endif // COMMON_FILE