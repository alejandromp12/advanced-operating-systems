#include "include/threadsManager.h"
#include <stdlib.h>

void populateWorker(thread *pWorker, int *pTickets, int totalTickets, int startTerm, int workLoad, int quantum, int threadId)

{
	//sanity check
	if (pWorker != NULL)
	{
		printf("totalTickets: %i,\n workLoad: %i,\n quantum: %i\n startTerm %i\n", totalTickets, workLoad, quantum, startTerm);
		pWorker->pTickets = pTickets;
		pWorker->totalTickets = totalTickets;
		pWorker->startTerm = startTerm;
		pWorker->workLoad = workLoad;
		pWorker->workLoadProgress = 0;
		pWorker->quantum = quantum;
		pWorker->piApprox = 0;
		pWorker->threadId = threadId;
	}
}


int updateWorkLoad(thread *pWorker, int newWorkLoad)
{
	int currenWorkLoad = -1;

	//sanity check
	if (pWorker == NULL)
	{
		printf("Error, updateWorkLoad(...) detected a Null pointer.\n");
		return currenWorkLoad;
	}

	pWorker->workLoadProgress += newWorkLoad;
	currenWorkLoad = pWorker->workLoadProgress / pWorker->workLoad;

	return (currenWorkLoad / 100);
}


void sleepWorker(thread *pWorker)
{
	//sanity check
	if (pWorker == NULL)
	{
		printf("Error, sleepWorker(...) detected a Null pointer.\n");
		return;
	}

	sigsetjmp(pWorker->sigjmpBuf, 1);
}


void wakeupWorker(thread *pWorker)
{
	//sanity check
	if (pWorker == NULL)
	{
		printf("Error, wakeupWorker(...) detected a Null pointer.\n");
		return;
	}

	siglongjmp(pWorker->sigjmpBuf, 1);
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