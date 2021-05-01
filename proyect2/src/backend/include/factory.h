#ifndef FACTORY_FILE
#define FACTORY_FILE

#include <semaphore.h>
#include <stdio.h>


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
	bufferElement *pBufferElements;
	int size;
	int bufferId;
} sharedBuffer;


int createSharedBuffer(int bufferSize, int bufferId);

sharedBuffer *_pSharedBuffer;

#endif // FACTORY_FILE