#include "include/common.h"

#include <string.h>

// Concatenates a char base name with an integer
char *getBufferName(char *baseName, int id)
{
	char bufferName[50];
	strcpy(bufferName, baseName);

	char intToString[5];
	sprintf(intToString, "%d", id);

	return strcat(bufferName, intToString);
}


/**
 * \brief Returns the random exponential distribution of a lambda value
 *
 * \param lambda, value between 0 and 1
 *
 * \return random exponential distribution of the lambda value passed
 */
double randomExponentialDistribution(double lambda)
{
	double intermediate = 0.0;
	intermediate = rand() / (RAND_MAX + 1.0);
	return -log(1 - intermediate) / lambda;
}


int getCounter(productConsumer counter, productConsumerRole role)
{
	// will be overwritten if needed
	int count = -1;
	switch (role)
	{
		case PRODUCER_ROLE:
		{
			sem_wait(&counter.producersMutex);
			count = counter.producers;
			sem_post(&counter.producersMutex);
			break;
		}

		case CONSUMER_ROLE:
		{
			sem_wait(&counter.consumersMutex);
			count = counter.consumers;
			sem_post(&counter.consumersMutex);
			break;
		}

		case NONE_ROLE:
		default:
			break;
	}

	return count;
}


void doActionToCounter(productConsumer counter, productConsumerRole role, productConsumerAction action)
{
	switch (role)
	{
		case PRODUCER_ROLE:
		{
			sem_wait(&counter.producersMutex);

			if (action == INCREASE)
			{
				++counter.producers;
			}
			else if ((action == DECREASE) && (counter.producers > 0))
			{
				--counter.producers;
			}

			sem_post(&counter.producersMutex);

			break;
		}

		case CONSUMER_ROLE:
		{
			sem_wait(&counter.consumersMutex);

			if (action == INCREASE)
			{
				++counter.consumers;
			}
			else if ((action == DECREASE) && (counter.consumers > 0))
			{
				--counter.consumers;
			}

			sem_post(&counter.consumersMutex);

			break;
		}

		default:
			break;
	}
}
