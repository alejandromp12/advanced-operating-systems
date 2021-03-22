#include "include/threadsManager.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

int populateWorker(thread *pWorker, int *pTickets, int totalTickets, int startTerm, int workLoad, int quantum, int threadId, sigjmp_buf environment)
{
	//sanity check
	if (pWorker == NULL)
	{
		printf("Error, populateWorker(...) detected a Null pointer.\n");
		return THREAD_ERROR;
	}

	printf("totalTickets: %i, workLoad: %i, quantum: %i, threadId: %i, startTerm: %i\n", totalTickets, workLoad, quantum, threadId, startTerm);
	pWorker->pTickets = pTickets;
	pWorker->totalTickets = totalTickets;
	pWorker->startTerm = startTerm;
	pWorker->currTerm = startTerm;
	pWorker->workLoad = workLoad;
	pWorker->workLoadProgress = 0.0;
	pWorker->quantum = quantum;
	pWorker->piApprox = 0;
	pWorker->threadId = threadId;
	pWorker->isPlaying = 1;
	memcpy(&(pWorker->environment), &environment, sizeof(sigjmp_buf));
	printf("totalTickets: %i, workLoad: %i, quantum: %i, threadId: %i, startTerm: %i\n", pWorker->totalTickets, pWorker->workLoad, pWorker->quantum, pWorker->threadId, pWorker->startTerm);

	return THREAD_NO_ERROR;
}


int getWorkLoadProgressInPercentage(thread *pWorker)
{
	int currenWorkLoad = THREAD_ERROR;

	//sanity check
	if (pWorker == NULL)
	{
		printf("Error, updateWorkLoad(...) detected a Null pointer.\n");
		return currenWorkLoad;
	}

	currenWorkLoad = pWorker->workLoadProgress / pWorker->workLoad;

	return (currenWorkLoad * 100);
}


void piCalculate(thread *pWorker, int isNonExpropiated)
{
	//sanity check
	if (pWorker == NULL)
	{
		printf("Error, piCalculate(...) detected a Null pointer.\n");
		return;
	}

	double termValue = -1;
	if (isNonExpropiated == 1)
	{
		double f = ((double)pWorker->quantum) / (double)100;
		double conditionalIndex = (double)pWorker->startTerm + ((double)pWorker->workLoad * (double)UNIT_OF_WORK) * f;
		double i = 0.0;
		for (i = (double)pWorker->currTerm; (i < conditionalIndex) && (pWorker->workLoadProgress < (double)pWorker->workLoad); i++)
		{
			termValue = 4 * (2 / ((4 * i + 1) * (4 * i + 3)));
			TOTAL_PI += termValue;

			// update workload progress
			pWorker->workLoadProgress += (double)pWorker->workLoad * ((double)1 / (double)UNIT_OF_WORK);
			pWorker->currTerm += 1;
		}
	}
	else
	{
		double conditionalIndex = (double)pWorker->startTerm + (double)pWorker->workLoad * (double)UNIT_OF_WORK;
		if (((double)pWorker->currTerm < conditionalIndex) && (pWorker->workLoadProgress < (double)pWorker->workLoad))
		{
			termValue = (double)4 * ((double)2 / (((double)4 * (double)pWorker->currTerm + (double)1) * ((double)4 * (double)pWorker->currTerm + (double)3)));
			TOTAL_PI += termValue;
			pWorker->currTerm += 1;

			// update workload progress
			pWorker->workLoadProgress += ((double)1 / (double)UNIT_OF_WORK);
		}
	}

	// overflow controll
	if (pWorker->workLoadProgress > (double)pWorker->workLoad)
	{
		pWorker->workLoadProgress = (double)pWorker->workLoad;
	}
}

