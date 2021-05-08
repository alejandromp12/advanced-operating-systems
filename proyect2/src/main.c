#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include "backend/include/consumer.h"
#include "backend/include/common.h"
#include "backend/include/factory.h"
#include "backend/include/finisher.h"
#include "backend/include/producer.h"
#include "backend/include/producerConsumerManager.h"
#if defined(CREATOR_APP)
#include "frontend/include/gui.h"
#endif

#if defined(CREATOR_APP)

int main(int argc, char  *argv[])
{
	// sanity check
	if (!argv)
	{
		printf("Error: argv is NULL.\n");
		return 1;
	}

	int PRODUCER_ARGS_NUM = 3;

	if (argc != PRODUCER_ARGS_NUM)
	{
		printf("Usage: $ %s Buffer_ID(int) Buffer_Size(int)\n", argv[0]);
		return 1;
	}

	int bufferId = atoi(argv[1]);
	int bufferSize = atoi(argv[2]);

	printf("CREATOR_APP.\n");

	if (!createSharedBuffer(bufferSize, bufferId))
	{
		printf("Error, creating shared buffer.\n");
		return 1;
	}

	while (1);

    // ends

	return 0;
}

#elif defined(PRODUCER_APP)

int main(int argc, char  *argv[])
{
	// sanity check
	if (!argv)
	{
		printf("Error: argv is NULL.\n");
		return 1;
	}

	int PRODUCER_ARGS_NUM = 3;

	if (argc != PRODUCER_ARGS_NUM)
	{
		printf("Usage: $ %s Buffer_ID(int) Average_Time_To_Send(int)\n", argv[0]);
		return 1;
	}

	int bufferId = atoi(argv[1]);
	double averageTime = atoi(argv[2]);

	double lambda = 1/averageTime;

	// Intializes time for random number generator
	time_t t;
	srand((unsigned)time(&t));

	int waitTime = 0;


	// simple test, just for review functionality
	printf("PRODUCER_APP.\n");

	char sharedBufferName[50];
	strcpy(sharedBufferName, getFixedName(SHARED_BUFFER_NAME, bufferId));

	producerProcess producer;
	producer.pid = getpid();
	producer.indexWrite = 0;
	producer.writtenMessage = 0;
	strcpy(producer.sharedBufferName, sharedBufferName);

	addProducerConsumer(PRODUCER_ROLE, sharedBufferName);
	insertProducerConsumerPIDToList(sharedBufferName, producer.pid, PRODUCER_ROLE);

	dataMessage data;
	data.producerId = getProducerConsumer(PRODUCER_ROLE, sharedBufferName);
	time_t rawTime;
	struct tm *infoTime;
	time(&rawTime);
	infoTime = localtime(&rawTime);
	strftime(data.date, sizeof(data.date), "%x - %I:%M%p", infoTime);

    while (1)
    {
		waitTime = ceil(getRandomExponentialDistribution(lambda));
		sleep(waitTime);

		infoTime = localtime(&rawTime);
		strftime(data.date, sizeof(data.date), "%x - %I:%M%p", infoTime);
		data.key = rand() % 5;
		tryWrite(data, &producer);
    }


	return 0;
}

#elif defined(CONSUMER_APP)

int main(int argc, char  *argv[])
{
	// sanity check
	if (!argv)
	{
		printf("Error: argv is NULL.\n");
		return 1;
	}

	int CONSUMER_ARGS_NUM = 3;

	if (argc != CONSUMER_ARGS_NUM)
	{
		printf("Usage: $ %s Buffer_ID(int) Average_Time_To_Read(int)\n", argv[0]);
		return 1;
	}

	int bufferId = atoi(argv[1]);
	double averageTime = atoi(argv[2]);

	double lambda = 1/averageTime;

	// Intializes time for random number generator
	time_t t;
	srand((unsigned)time(&t));

	int waitTime = 0;

	// simple test, just for review functionality
	printf("CONSUMER_APP.\n");
    // ends

    char sharedBufferName[50];
	strcpy(sharedBufferName, getFixedName(SHARED_BUFFER_NAME, bufferId));

	consumerProcess consumer;
	consumer.pid = getpid();
	consumer.readIndex = 0;
	consumer.readMessage = 0;
	strcpy(consumer.sharedBufferName, sharedBufferName);

	addProducerConsumer(CONSUMER_ROLE, sharedBufferName);
	insertProducerConsumerPIDToList(sharedBufferName, consumer.pid, CONSUMER_ROLE);

	while (1)
    {
		waitTime = ceil(getRandomExponentialDistribution(lambda));
		sleep(waitTime);

		tryRead(&consumer);
	}

	return 0;
}

#elif defined(TERMINATOR_APP)

int main(int argc, char  *argv[])
{
	// sanity check
	if (!argv)
	{
		printf("Error: argv is NULL.\n");
		return 1;
	}

	int TERMINATOR_ARGS_NUM = 2;

	if (argc != TERMINATOR_ARGS_NUM)
	{
		printf("Usage: $ %s Buffer_ID(int) \n", argv[0]);
		return 1;
	}

	int bufferId = atoi(argv[1]);

	// simple test, just for review functionality
	printf("TERMINATOR_APP.\n");

	char sharedBufferName[50];
	strcpy(sharedBufferName, getFixedName(SHARED_BUFFER_NAME, bufferId));
	if (!killProducer(sharedBufferName))
	{
		printf("Error, kill producer.\n");
		return 1;
	}

	// temp
	printf("Press Enter to Continue:");  
	getchar();
	if (!removeBuffer(sharedBufferName))
	{
		printf("Error, removing buffer.\n");
		return 1;
	}
	
    // ends

	return 0;
}

#endif

