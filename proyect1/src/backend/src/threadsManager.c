#include "include/threadsManager.h"
#include <stdlib.h>
#include <string.h>

int populateWorker(thread *pWorker, int *pTickets, int totalTickets, int workLoad, int quantum, int threadId, sigjmp_buf environment)
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

