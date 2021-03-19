#include <stdio.h>
#include <stdlib.h>
#include "backend/include/threadsManager.h"
#include "backend/include/scheduler.h"

#define TOTAL_TICKETS 100

int main(int argc, char *pArgv[])
{
	int numWorkers = 5;
	int tickets1[7] = {9, 50, 3, 1, 4, 0, 2};
	int tickets2[2] = {8, 51};
	int tickets3[6] = {10, 45, 15, 19, 31, 21};
	int tickets4[3] = {85, 99, 80};
	int tickets5[1] = {100};

	thread *pWorker = (thread*)malloc(numWorkers * sizeof(thread));
	if (pWorker == NULL)
	{
		printf("Error, main(...) detected a Null pointer.\n");
	}

	for (int i = 0; i < numWorkers; i++)
	{
		if (i == 0)
		{
			populateWorker(&pWorker[i], tickets1, 7, 3, 1, i);
		}
		else if (i == 1)
		{
			populateWorker(&pWorker[i], tickets2, 2, 3, 1, i);
		}
		else if (i == 2)
		{
			populateWorker(&pWorker[i], tickets3, 6, 3, 1, i);
		}
		else if (i == 3)
		{
			populateWorker(&pWorker[i], tickets4, 3, 3, 1, i);
		}
		else if (i == 4)
		{
			populateWorker(&pWorker[i], tickets5, 1, 3, 1, i);
		}
	}

	scheduler systemScheduler;
	int *pTickets = (int*)malloc(TOTAL_TICKETS * sizeof(int));
	initializeScheduler(&systemScheduler, EXPROPRIATED_MODE, pTickets, TOTAL_TICKETS);
	validateTickets(tickets1, 7, pTickets);
	validateTickets(tickets2, 2, pTickets);
	validateTickets(tickets3, 6, pTickets);
	validateTickets(tickets4, 3, pTickets);
	validateTickets(tickets5, 1, pTickets);

	while (haveValidTickets(pTickets) > 0)
	{
		lotteryChooseNextWorker(&systemScheduler, pWorker, numWorkers, pTickets);
		// simulate that the worker has to be removed
		thread *pThread = (thread*)systemScheduler.pPrevWorker;
		if (pThread != NULL)
		{
			// remove its tickest
			invalidateTickets(pThread->pTickets, pThread->totalTickets, pTickets);
		}
	}

	free(pWorker);
	free(pTickets);
	return 0;
}

