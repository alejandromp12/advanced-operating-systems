#include "include/threadsManager.h"
#include <stdlib.h>

void populateWorker(thread *pWorker, int *tickets, int totalTickets, int startTerm, int workLoad, int quantum)
{
	if (pWorker != NULL)
	{
		printf("totalTickets: %i,\n workLoad: %i,\n quantum: %i\n startTerm %i\n", totalTickets, workLoad, quantum, startTerm);
		pWorker->tickets = tickets;
		pWorker->totalTickets = totalTickets;\
		pWorker->startTerm = startTerm;
		pWorker->workLoad = workLoad;
		pWorker->workLoadProgress = 0;
		pWorker->quantum = quantum;
		pWorker->piApprox = 0;
	}
}


int updateWorkLoad(thread *pWorker, int newWorkLoad)
{
	int currenWorkLoad = 0;
	if (pWorker != NULL)
	{
		pWorker->workLoadProgress += newWorkLoad;
		currenWorkLoad = pWorker->workLoadProgress / pWorker->workLoad;
	}

	return (currenWorkLoad / 100);
}


void sleepWorker(thread *pWorker)
{
	if (pWorker != NULL)
	{
		sigsetjmp(pWorker->sigjmpBuf, 1);
	}
}


void wakeupWorker(thread *pWorker)
{
	if (pWorker != NULL)
	{
		siglongjmp(pWorker->sigjmpBuf, 1);
	}
}


void piCalculate(thread *pWorker)
{
	int conditionalIndex = pWorker->startTerm + (pWorker->workLoad * UNIT_OF_WORK);
	double termValue = -1;

	for(int i = pWorker->startTerm; i < conditionalIndex; i++)
	{	
		termValue = 4 * (2 / ((4 * i + 1) * (4 * i + 3)));
		TOTAL_PI += termValue;
	}

}