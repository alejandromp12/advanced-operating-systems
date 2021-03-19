#include "include/scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void initializeScheduler(scheduler *pScheduler, OperationModeEnum mode, int totalTickets)
{
	if (pScheduler != NULL)
	{
		printf("totalTickets: %i,\n operationMode: %i\n", totalTickets, mode);
		pScheduler->totalBaseTickets = totalTickets;
		pScheduler->mode = mode;
		pScheduler->pNextWorker = NULL;
	}
}


LotteryResultEnum lotteryChooseNextWorker(scheduler *pScheduler, thread *pWorkers, int workers)
{
	LotteryResultEnum processResult = ERROR;
	if ((pScheduler != NULL) && (pWorkers != NULL))
	{
		// TODO
		//compute effective tickets based on
		//ticket_boost or frac. of quanta used;
		//convert effective tickets to base tickets
		//based on user's exchange rate;
		//use threshold if process is nice'd;
		//update our running count of base tickets;

		// Intializes random number generator
		time_t t;
		srand((unsigned)time(&t));
		int randomTicket = rand() % pScheduler->totalBaseTickets;
		for (int i = 0; i < workers; i++)
		{
			if (&pWorkers[i] != NULL)
			{
				int totalWorkerTickets = (&pWorkers[i])->totalTickets;
				for (int j = 0; j < totalWorkerTickets; j++)
				{
					// check if we have a winner
					if ((&pWorkers[i])->tickets[j] == randomTicket)
					{
						processResult = NO_ERROR;
						pScheduler->pNextWorker = &pWorkers[i];
					}
				}
			}
		}
	}

	return processResult;
}
