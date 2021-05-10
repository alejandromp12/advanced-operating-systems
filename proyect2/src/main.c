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
#include <sys/time.h>

#include "backend/include/consumer.h"
#include "backend/include/common.h"
#include "backend/include/factory.h"
#include "backend/include/finisher.h"
#include "backend/include/producer.h"
#include "backend/include/producerConsumerManager.h"
#include "backend/include/scheduler.h"
#if defined(CREATOR_APP)
#include "frontend/include/gui.h"
#endif

#if defined(CREATOR_APP)

char _sharedBufferName[50];
sharedBuffer *_pSharedBuffer;

void updateBufferElementGUI()
{	
	char tmp[100];
	time_t t;
	srand((unsigned)time(&t));

	struct stat buffer;
	if (!stat(_sharedBufferName, &buffer) && (_pSharedBuffer != NULL))
	{
		for(int i = 0; i < _bufferSizeGUI; i++)
		{	
			sem_wait(&(_pSharedBuffer->bufferElements[i].mutex));
			if (_pSharedBuffer->bufferElements[i].data.producerId == -1)
			{
				gtk_label_set_text(GTK_LABEL(bufferGUI[i].producerIdLabel), "No PID");
			}
			else
			{
				sprintf(tmp, "%i", _pSharedBuffer->bufferElements[i].data.producerId);
				gtk_label_set_text(GTK_LABEL(bufferGUI[i].producerIdLabel), tmp);
			}

			if (strcmp(_pSharedBuffer->bufferElements[i].data.date, "") == 0)
			{
				gtk_label_set_text(GTK_LABEL(bufferGUI[i].dateLabel), "No Date");
			}
			else
			{
				sprintf(tmp, "%s", _pSharedBuffer->bufferElements[i].data.date);
				gtk_label_set_text(GTK_LABEL(bufferGUI[i].dateLabel), tmp);
			}

			if (_pSharedBuffer->bufferElements[i].data.key == -1)
			{
				gtk_label_set_text(GTK_LABEL(bufferGUI[i].keyLabel), "No Key");
			}
			else
			{
				sprintf(tmp, "%i", _pSharedBuffer->bufferElements[i].data.key);
				gtk_label_set_text(GTK_LABEL(bufferGUI[i].keyLabel), tmp);
			}
			sem_post(&(_pSharedBuffer->bufferElements[i].mutex));

			sprintf(tmp, "");
		}
	}
}


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

	void (*ptr)() = &updateBufferElementGUI;
	_ptrUpdateGUI = ptr;
	_bufferSizeGUI = bufferSize;
	_bufferId = bufferId;

	strcpy(_sharedBufferName, getFixedName(SHARED_BUFFER_NAME, bufferId));
	_pSharedBuffer = getSharedBuffer(_sharedBufferName);
	if (_pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return 1;
	}

	runGUI(argc, argv, bufferId);


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
	
	struct stat buffer;
	if (stat(sharedBufferName, &buffer) != 0)
	{
		printf("Error, buffer %s does not exist.\n", sharedBufferName);
		return 1;
	}

	producerProcess producer;
	producer.pid = getpid();
	producer.indexWrite = 0;
	producer.timeSpendInMutex = 0;
	producer.idleTime = 0;
	producer.writtenMessages = 0;
	strcpy(producer.sharedBufferName, sharedBufferName);

	addProducerConsumer(PRODUCER_ROLE, sharedBufferName);
	insertProducerConsumerPIDToList(sharedBufferName, producer.pid, PRODUCER_ROLE);

	dataMessage data;
	data.producerId = producer.pid;
	producer.producerId = data.producerId;

	time_t rawTime;
	struct tm *infoTime;
	time(&rawTime);
	infoTime = localtime(&rawTime);
	strftime(data.date, sizeof(data.date), "%x - %X", infoTime);

	gettimeofday(&producer.startTime, NULL);
    while (1)
    {
		waitTime = ceil(getRandomExponentialDistribution(lambda));
		sleep(waitTime);
		time(&rawTime);

		infoTime = localtime(&rawTime);
		strftime(data.date, sizeof(data.date), "%x - %X", infoTime);
		data.key = rand() % 5;
		data.killerPID = NO_PID;
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

	struct stat buffer;
	if (stat(sharedBufferName, &buffer) != 0)
	{
		printf("Error, buffer %s does not exist.\n", sharedBufferName);
		return 1;
	}

	consumerProcess consumer;
	consumer.pid = getpid();
	consumer.readIndex = 0;
	consumer.readMessages = 0;
	consumer.killerPID = 0;
	consumer.timeSpendInMutex = 0;
	consumer.idleTime = 0;
	strcpy(consumer.sharedBufferName, sharedBufferName);

	addProducerConsumer(CONSUMER_ROLE, sharedBufferName);
	insertProducerConsumerPIDToList(sharedBufferName, consumer.pid, CONSUMER_ROLE);

	consumer.consumerId = getProducerConsumer(CONSUMER_ROLE, sharedBufferName);
	gettimeofday(&consumer.startTime, NULL);
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

	struct stat buffer;
	if (stat(sharedBufferName, &buffer) != 0)
	{
		printf("Error, buffer %s does not exist.\n", sharedBufferName);
		return 1;
	}

	int waitCondition = 1;
	do
	{
		waitCondition = 0;
		if (killProducers(sharedBufferName) == 1)
		{
			//wakeup(sharedBufferName, PRODUCER_ROLE);
			waitCondition = (producersStillAlive(sharedBufferName) == 1) ? 1 : 0;
		}
	} while (waitCondition);

	waitCondition = 1;
	do
	{
		waitCondition = 0;
		if (killConsumers(sharedBufferName) == 1)
		{
			//wakeup(sharedBufferName, CONSUMER_ROLE);
			waitCondition = (consumersStillAlive(sharedBufferName) == 1) ? 1 : 0;
		}
	} while (waitCondition);


	logTerminatorAction(sharedBufferName, 0, NULL, REMOVE_SHARED_BUFFER);
	if (!removeBuffer(sharedBufferName))
	{
		printf("Error, removing buffer.\n");
		return 1;
	}

    // ends

	return 0;
}

#endif

