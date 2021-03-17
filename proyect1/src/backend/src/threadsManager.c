#include "include/threadsManager.h"

/**
 *
 */
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


