#include "include/threadsManager.h"


void populateWorker(void *pThread, int tickets, int workLoad, int quantum, OperationModeEnum mode)
{
	thread pWorker = (thread)pThread;
	if (pWorker)
	{
		pWorker->tickets = tickets;
		pWorker->workLoad = workLoad;
		pWorker->workLoadProgress = 0;
		pWorker->quantum = quantum;
		pWorker->piApprox = 0;
		pWorker->mode = mode;
	}
}


ResultEnum updateQuantum(void *pThread, int newQuantum)
{
	ResultEnum result = ERROR;
	thread pWorker = (thread)pThread;
	if (pWorker)
	{
		pWorker->quantum = newQuantum;
		result = NO_ERROR;
	}

	return result;
}


int updateWorkLoad(void *pThread, int newWorkLoad)
{
	int currenWorkLoadPercentage = 0;
	thread pWorker = (thread)pThread;
	if (pWorker)
	{
		pWorker->workLoadProgress += newWorkLoad;
		currenWorkLoadPercentage = pWorker->workLoadProgress / pWorker->workLoad;
	}

	return (currenWorkLoadPercentage / 100);
}
