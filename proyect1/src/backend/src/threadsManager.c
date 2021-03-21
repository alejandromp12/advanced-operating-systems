#include "include/threadsManager.h"
#include <stdlib.h>
#include <string.h>

int populateWorker(thread *pWorker, int *pTickets, int totalTickets, int startTerm,int workLoad, int quantum, int threadId, sigjmp_buf environment)
{
	//sanity check
	if (pWorker == NULL)
	{
		printf("Error, populateWorker(...) detected a Null pointer.\n");
		return THREAD_ERROR;
	}

	printf("totalTickets: %i, workLoad: %i, quantum: %i, threadId: %i\n", totalTickets, workLoad, quantum, threadId);
	pWorker->pTickets = pTickets;
	pWorker->totalTickets = totalTickets;
	pWorker->startTerm = startTerm;
	pWorker->workLoad = workLoad;
	pWorker->workLoadProgress = 0;
	pWorker->quantum = quantum;
	pWorker->piApprox = 0;
	pWorker->threadId = threadId;
	memcpy(&(pWorker->environment), &environment, sizeof(sigjmp_buf));

	return THREAD_NO_ERROR;
}


int updateWorkLoad(thread *pWorker, int newWorkLoad)
{
	int currenWorkLoad = THREAD_ERROR;

	//sanity check
	if (pWorker == NULL)
	{
		printf("Error, updateWorkLoad(...) detected a Null pointer.\n");
		return currenWorkLoad;
	}

	pWorker->workLoadProgress += newWorkLoad;
	currenWorkLoad = pWorker->workLoadProgress / pWorker->workLoad;

	return (currenWorkLoad * 100);
}

void piCalculate(thread *pWorker)
{
	int conditionalIndex = pWorker->startTerm + (pWorker->workLoad * UNIT_OF_WORK);
	double termValue = -1;

	for(double i = pWorker->startTerm; i < conditionalIndex; i++)
	{	
		termValue = 4 * (2 / ((4 * (double)i + 1) * (4 * (double)i + 3)));
		TOTAL_PI += termValue;
	}

	printf("%f\n", TOTAL_PI);
}

