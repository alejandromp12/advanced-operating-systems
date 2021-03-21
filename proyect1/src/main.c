#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "backend/include/threadsManager.h"
#include "backend/include/scheduler.h"

#define TOTAL_TICKETS 100
int *_pTickets;

static void lotteryThreads(scheduler *pSystemScheduler, thread *pWorkers, int numWorkers)
{
	if ((pSystemScheduler == NULL) || (pWorkers == NULL) || (_pTickets == NULL))
	{
		printf("Error, lotteryThreads(...) detected a Null pointer.\n");
		return;
	}

	int stopLotteryThreads = 0;
	while ((haveValidTickets(_pTickets) > 0) && (stopLotteryThreads == 0))
	{
		// verify if the worker has to be removed, we have to this before any attempt to playing again
		thread *pPreviousWorker = (thread*)pSystemScheduler->pPrevWorker;
		if (pPreviousWorker != NULL)
		{
			// if it already finishes its work, the remove it
			if (pPreviousWorker->workLoadProgress == pPreviousWorker->workLoad)
			{
				// remove its tickest
				if (invalidateTickets(pPreviousWorker->pTickets, pPreviousWorker->totalTickets, _pTickets)  != SCHEDULER_NO_ERROR)
				{
					stopLotteryThreads = 1;
					continue;
				}
			}
		}

		if (lotteryChooseNextWorker(pSystemScheduler, pWorkers, numWorkers, _pTickets) == SCHEDULER_NO_ERROR)
		{
			thread *pCurrWorker = (thread*)pSystemScheduler->pNextWorker;
			if (pCurrWorker == NULL)
			{
				stopLotteryThreads = 1;
				continue;
			}

			if (sigsetjmp(pSystemScheduler->environment, 1) == 0)
			{
				printf("Worker %i just woke up.\n", pCurrWorker->threadId);
				siglongjmp(pCurrWorker->environment, pCurrWorker->threadId);
				// if we reach here is because of there was an error
				printf("Error context, remove me just for testing.\n");
				stopLotteryThreads = 1;
				continue;
			}
			else
			{
				printf("Scheduler got back the control, remove me just for testing.\n");
			}
		}
	}
}


int main(int argc, char *pArgv[])
{
	// Intializes time for random number generator
	time_t t;
	srand((unsigned)time(&t));

	int numWorkers = 5;
	int tickets1[7] = {9, 50, 3, 1, 4, 0, 2};
	int tickets2[2] = {8, 51};
	int tickets3[6] = {10, 45, 15, 19, 31, 21};
	int tickets4[3] = {85, 99, 80};
	int tickets5[1] = {100};

	_pTickets = (int*)malloc(TOTAL_TICKETS * sizeof(int));
	if (_pTickets == NULL)
	{
		printf("Error, main(...) detected a Null pointer.\n");
		return 0;
	}

	sigjmp_buf schedulerEnvironment;
	scheduler systemScheduler;
	if ((initializeScheduler(&systemScheduler, EXPROPRIATED_MODE, _pTickets, TOTAL_TICKETS, schedulerEnvironment) != SCHEDULER_NO_ERROR)
		|| (validateTickets(tickets1, 7, _pTickets) != SCHEDULER_NO_ERROR)
		|| (validateTickets(tickets2, 2, _pTickets) != SCHEDULER_NO_ERROR)
		|| (validateTickets(tickets3, 6, _pTickets) != SCHEDULER_NO_ERROR)
		|| (validateTickets(tickets4, 3, _pTickets) != SCHEDULER_NO_ERROR)
		|| (validateTickets(tickets5, 1, _pTickets) != SCHEDULER_NO_ERROR))
	{
		printf("Error, validating tickets or initializing scheduler.\n");
		return 0;
	}

	thread *pWorkers = (thread*)malloc(numWorkers * sizeof(thread));
	if (pWorkers == NULL)
	{
		printf("Error, main(...) detected a Null pointer.\n");
		return 0;
	}

	sigjmp_buf workersEnvironment[numWorkers];
	int tmpResult;
	for (int i = 0; i < numWorkers; i++)
	{
		if (i == 0)
		{
			tmpResult = populateWorker(&pWorkers[i], tickets1, 7, 6000, 1000, i + 1, workersEnvironment[i]);
		}
		else if (i == 1)
		{
			tmpResult = populateWorker(&pWorkers[i], tickets2, 2, 8000, 1000, i + 1, workersEnvironment[i]);
		}
		else if (i == 2)
		{
			tmpResult = populateWorker(&pWorkers[i], tickets3, 6, 9000, 2000, i + 1, workersEnvironment[i]);
		}
		else if (i == 3)
		{
			tmpResult = populateWorker(&pWorkers[i], tickets4, 3, 10000, 5000, i + 1, workersEnvironment[i]);
		}
		else if (i == 4)
		{
			tmpResult = populateWorker(&pWorkers[i], tickets5, 1, 50000, 1500, i + 1, workersEnvironment[i]);
		}

		if (tmpResult != THREAD_NO_ERROR)
		{
			printf("Error, initializing workers.\n");
			return 0;
		}
	}

	prepareWorkersEnvironment(pWorkers, &systemScheduler, numWorkers);
	lotteryThreads(&systemScheduler, pWorkers, numWorkers);

	if (pWorkers != NULL)
	{
		free(pWorkers);
	}

	if (_pTickets != NULL)
	{
		free(_pTickets);
	}

	return 0;
}

