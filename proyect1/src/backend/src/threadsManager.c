#include "include/threadsManager.h"


void populateWorker(void *pThread, int tickets, int workLoad, int quantum, OperationModeEnum mode)
{
	thread pWorker = (thread)pThread;
	if (pWorker)
	{
		pWorker->tickets = tickets;
		pWorker->workLoad = workLoad;
		pWorker->quantum = quantum;
		pWorker->mode = mode;
	}
}


void updateQuantum(void *pThread, int newQuantum)
{
	thread pWorker = (thread)pThread;
	if (pWorker)
	{
		pWorker->quantum = newQuantum;
	}
}


void updateWorkLoad(void *pThread, int newWorkLoad)
{
	thread pWorker = (thread)pThread;
	if (pWorker)
	{
		pWorker->workLoad = newWorkLoad;
	}
}
