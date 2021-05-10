#include "include/producerConsumerManager.h"


int getProducerConsumer(producerConsumerRole role, char *bufferName)
{
	// will be overwritten if needed
	int count = -1;

	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return count;
	}

	switch (role)
	{
		case PRODUCER_ROLE:
		{
			sem_wait(&(pSharedBuffer->counter.producersMutex));
			count = pSharedBuffer->counter.producers;
			sem_post(&(pSharedBuffer->counter.producersMutex));
			break;
		}

		case CONSUMER_ROLE:
		{
			sem_wait(&(pSharedBuffer->counter.consumersMutex));
			count = pSharedBuffer->counter.consumers;
			sem_post(&(pSharedBuffer->counter.consumersMutex));
			break;
		}

		default:
			break;
	}

	return count;
}


int doActionToCounter(sharedBuffer *pSharedBuffer, producerConsumerRole role, producerConsumerAction action)
{
	int result = -1;
	switch (role)
	{
		case PRODUCER_ROLE:
		{
			sem_wait(&(pSharedBuffer->counter.producersMutex));

			if (action == INCREASE)
			{
				++(pSharedBuffer->counter.producers);
			}
			else if ((action == DECREASE) && (pSharedBuffer->counter.producers > 0))
			{
				--(pSharedBuffer->counter.producers);
			}
			
			result = pSharedBuffer->counter.producers;

			sem_post(&(pSharedBuffer->counter.producersMutex));

			break;
		}

		case CONSUMER_ROLE:
		{
			sem_wait(&(pSharedBuffer->counter.consumersMutex));

			if (action == INCREASE)
			{
				++(pSharedBuffer->counter.consumers);
			}
			else if ((action == DECREASE) && (pSharedBuffer->counter.consumers > 0))
			{
				--(pSharedBuffer->counter.consumers);
			}

			result = pSharedBuffer->counter.consumers;

			sem_post(&(pSharedBuffer->counter.consumersMutex));

			break;
		}

		default:
			break;
	}

	return result;
}


int addProducerConsumer(producerConsumerRole role, char *bufferName)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return -1;
	}

	return doActionToCounter(pSharedBuffer, role, INCREASE);
}

int removeProducerConsumer(producerConsumerRole role, char *bufferName)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return -1;
	}

	return doActionToCounter(pSharedBuffer, role, DECREASE);
}


void logProducerConsumerAction(char *bufferName, producerConsumerRole role, int bufferIndex)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return;
	}

	// perform logging of the process
	char log[200];
	sprintf(log,
		    "==================================\n"
		    "Message %s at index %d of the buffer %s\n"
		    "Consumers alive: %d\n"
		    "Producers alive: %d\n"
		    "==================================",
			(role == PRODUCER_ROLE) ? "produced" : "consumed",
			bufferIndex + 1,
			bufferName,
			getProducerConsumer(CONSUMER_ROLE, bufferName),
			getProducerConsumer(PRODUCER_ROLE, bufferName));

	doLogging(log, pSharedBuffer);

	//printf("Message logged:\n%s\n", log);
}

void setProducerConsumerPIDState(sharedBuffer *pSharedBuffer, int pid, producerConsumerPidState state, producerConsumerRole role)
{
	if (pSharedBuffer == NULL)
	{
		printf("Error, pSharedBuffer is NULL.\n");
		return;
	}

	switch (role)
	{
		case PRODUCER_ROLE:
		{
			sem_wait(&(pSharedBuffer->PIDs.producersPIDsMutex));

			for (int i = 0; i < MAX_PIDS; i++)
			{
				if (pid == pSharedBuffer->PIDs.producersPIDs[i][0])
				{
					pSharedBuffer->PIDs.producersPIDs[i][1] = state;
					break;
				}
			}

			sem_post(&(pSharedBuffer->PIDs.producersPIDsMutex));
			break;
		}

		case CONSUMER_ROLE:
		{
			sem_wait(&(pSharedBuffer->PIDs.consumersPIDsMutex));

			for (int i = 0; i < MAX_PIDS; i++)
			{
				if (pid == pSharedBuffer->PIDs.consumersPIDs[i][0])
				{
					pSharedBuffer->PIDs.consumersPIDs[i][1] = state;
					break;
				}
			}

			sem_post(&(pSharedBuffer->PIDs.consumersPIDsMutex));
			break;
		}

		default:
			break;
	}
}

void removeProducerConsumerPIDFromList(char *bufferName, int pid, producerConsumerRole role)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return;
	}

	switch (role)
	{
		case PRODUCER_ROLE:
		{
			sem_wait(&(pSharedBuffer->PIDs.producersPIDsMutex));

			for (int i = 0; i < MAX_PIDS; i++)
			{
				if (pid == pSharedBuffer->PIDs.producersPIDs[i][0])
				{
					pSharedBuffer->PIDs.producersPIDs[i][0] = NO_PID;
					pSharedBuffer->PIDs.producersPIDs[i][1] = INACTIVE;
					break;
				}
			}

			sem_post(&(pSharedBuffer->PIDs.producersPIDsMutex));
			break;
		}

		case CONSUMER_ROLE:
		{
			sem_wait(&(pSharedBuffer->PIDs.consumersPIDsMutex));

			for (int i = 0; i < MAX_PIDS; i++)
			{
				if (pid == pSharedBuffer->PIDs.consumersPIDs[i][0])
				{
					pSharedBuffer->PIDs.consumersPIDs[i][0] = NO_PID;
					pSharedBuffer->PIDs.consumersPIDs[i][1] = INACTIVE;
					break;
				}
			}

			sem_post(&(pSharedBuffer->PIDs.consumersPIDsMutex));
			break;
		}

		default:
			break;
	}
}

void insertProducerConsumerPIDToList(char *bufferName, int pid, producerConsumerRole role)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return;
	}

	switch (role)
	{
		case PRODUCER_ROLE:
		{
			sem_wait(&(pSharedBuffer->PIDs.producersPIDsMutex));

			for (int i = 0; i < MAX_PIDS; i++)
			{
				if (NO_PID == pSharedBuffer->PIDs.producersPIDs[i][0])
				{
					pSharedBuffer->PIDs.producersPIDs[i][0] = pid;
					pSharedBuffer->PIDs.consumersPIDs[i][1] = ACTIVE;
					break;
				}
			}

			sem_post(&(pSharedBuffer->PIDs.producersPIDsMutex));
			break;
		}

		case CONSUMER_ROLE:
		{
			sem_wait(&(pSharedBuffer->PIDs.consumersPIDsMutex));

			for (int i = 0; i < MAX_PIDS; i++)
			{
				if (NO_PID == pSharedBuffer->PIDs.consumersPIDs[i][0])
				{
					pSharedBuffer->PIDs.consumersPIDs[i][0] = pid;
					pSharedBuffer->PIDs.consumersPIDs[i][1] = ACTIVE;
					break;
				}
			}

			sem_post(&(pSharedBuffer->PIDs.consumersPIDsMutex));
			break;
		}

		default:
			break;
	}
}

int getInActiveProducerConsumerPID(sharedBuffer *pSharedBuffer, producerConsumerRole role)
{
	int pid = -1; // will be overwritten if needed
	switch (role)
	{
		case PRODUCER_ROLE:
		{
			sem_wait(&(pSharedBuffer->PIDs.producersPIDsMutex));

			for (int i = 0; i < MAX_PIDS; i++)
			{
				if (INACTIVE == pSharedBuffer->PIDs.producersPIDs[i][1] && pSharedBuffer->PIDs.producersPIDs[i][0] != -1)
				{
					pid = pSharedBuffer->PIDs.producersPIDs[i][0];
					break;
				}
			}

			sem_post(&(pSharedBuffer->PIDs.producersPIDsMutex));
			break;
		}

		case CONSUMER_ROLE:
		{
			sem_wait(&(pSharedBuffer->PIDs.consumersPIDsMutex));

			for (int i = 0; i < MAX_PIDS; i++)
			{
				if (INACTIVE == pSharedBuffer->PIDs.consumersPIDs[i][1] && pSharedBuffer->PIDs.consumersPIDs[i][0] != -1 )
				{
					pid = pSharedBuffer->PIDs.consumersPIDs[i][0];
					break;
				}
			}

			sem_post(&(pSharedBuffer->PIDs.consumersPIDsMutex));
			break;
		}

		default:
			break;
	}

	return pid;
}


int producersStillAlive(char *bufferName)
{
	int processesAlive = 1;
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return processesAlive;
	}

	processesAlive = 0;
	sem_wait(&(pSharedBuffer->PIDs.producersPIDsMutex));
	for (int i = 0; i < MAX_PIDS; i++)
	{
		if (NO_PID != pSharedBuffer->PIDs.producersPIDs[i][0])
		{
			processesAlive = 1;
			break;
		}
	}
	sem_post(&(pSharedBuffer->PIDs.producersPIDsMutex));

	return processesAlive;
}


int consumersStillAlive(char *bufferName)
{
	int processesAlive = 1;
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return processesAlive;
	}

	processesAlive = 0;
	sem_wait(&(pSharedBuffer->PIDs.consumersPIDsMutex));
	for (int i = 0; i < MAX_PIDS; i++)
	{
		if (NO_PID != pSharedBuffer->PIDs.consumersPIDs[i][0])
		{
			processesAlive = 1;
			break;
		}
	}
	sem_post(&(pSharedBuffer->PIDs.consumersPIDsMutex));

	return processesAlive;
}