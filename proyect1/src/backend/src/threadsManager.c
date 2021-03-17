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
	int currenWorkLoad = 0;
	thread pWorker = (thread)pThread;
	if (pWorker)
	{
		pWorker->workLoadProgress += newWorkLoad;
		currenWorkLoad = pWorker->workLoadProgress / pWorker->workLoad;
	}

	return (currenWorkLoad / 100);
}


void sleepWorker(void *pThread)
{
	thread pWorker = (thread)pThread;
	if (pWorker)
	{
		sigsetjmp(pWorker->sigjmpBuf, 1);
	}
}


void wakeupWorker(void *pThread)
{
	thread pWorker = (thread)pThread;
	if (pWorker)
	{
		siglongjmp(pWorker->sigjmpBuf, 1);
	}
}
