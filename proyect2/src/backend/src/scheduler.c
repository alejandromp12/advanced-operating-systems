#include "include/scheduler.h"

#include "include/common.h"
#include <sys/types.h>
#include <signal.h>


void wakeup2(sharedBuffer *pSharedBuffer, producerConsumerRole role)
{
	if (pSharedBuffer == NULL)
	{
		printf("Error, pSharedBuffer is NULL.\n");
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
					setProducerConsumerPIDState(pSharedBuffer, pid, ACTIVE, (role == PRODUCER_ROLE) ? PRODUCER_ROLE : CONSUMER_ROLE);
					doProcess(pid, CONTINUE);
				}
			}
			break;
		}

		default:
			break;
	}
}

void wakeup(char *bufferName, producerConsumerRole role)
{
	sharedBuffer *pSharedBuffer = getSharedBuffer(bufferName);
	if (pSharedBuffer == NULL)
	{
		printf("Error, getSharedBuffer() failed.\n");
		return;
	}

	wakeup2(pSharedBuffer, role);
}

void doProcess(int pid, processAction action)
{
	struct sigaction sigact;
	/* set up a signal catching function to handle the signals */
	/* that will be sent from the sendsig() function */
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigact.sa_handler = NULL;

	switch (action)
	{
		case STOP:
		{
			sigaction(SIGSTOP, &sigact, NULL);
			kill(pid, SIGSTOP);
			break;
		}

		case CONTINUE:
		{
			sigaction(SIGCONT, &sigact, NULL);
			kill(pid, SIGCONT);
			break;
		}

		case KILL:
		{
			sigaction(SIGTERM, &sigact, NULL);
			kill(pid, SIGTERM);
			break;
		}

		default:
			break;
	}
}