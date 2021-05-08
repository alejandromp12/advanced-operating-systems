#include "include/scheduler.h"

#include "include/common.h"
#include <sys/types.h>
#include <signal.h>


int bufferAvailableSpace(sharedBuffer *pSharedBuffer)
{
	int availableSpace = 0;
	for (int i = 0; i < pSharedBuffer->size; i++)
	{
    	sem_wait(&(pSharedBuffer->bufferElements[i].mutex));
    	if (pSharedBuffer->bufferElements[i].indexAvailable == 1)
    	{
    		++availableSpace;
    	}
    	sem_post(&(pSharedBuffer->bufferElements[i].mutex));
	}

    return availableSpace;
}


int isBufferFull(sharedBuffer *pSharedBuffer)
{
	return (pSharedBuffer->size == bufferAvailableSpace(pSharedBuffer));
}


int isBufferEmpty(sharedBuffer *pSharedBuffer)
{
	return (0 == bufferAvailableSpace(pSharedBuffer));
}

void wakeup(char *bufferName, producerConsumerRole role)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return;
	}

	int pid = 0;
	switch (role)
	{
		case PRODUCER_ROLE:
		case CONSUMER_ROLE:
		{
			while (pid != -1)
			{
				pid = getInActiveProducerConsumerPID(pSharedBuffer, role);

				if (pid != -1)
				{
					setProducerConsumerPIDState(bufferName, pid, ACTIVE, (role == PRODUCER_ROLE) ? PRODUCER_ROLE : CONSUMER_ROLE);
					doProcess(pid, CONTINUE);
				}
			}
			break;
		}

		default:
			break;
	}
}

void doProcess(int pid, processAction action)
{
	struct sigaction sigact;
	/* set up a signal catching function to handle the signals */
	/* that will be sent from the sendsig() function */
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigact.sa_handler = NULL;

	if (action == STOP)
	{
		sigaction(SIGSTOP, &sigact, NULL);
		kill(pid, SIGSTOP);
	}
	else
	{
		sigaction(SIGCONT, &sigact, NULL);
		kill(pid, SIGCONT);
	}
}