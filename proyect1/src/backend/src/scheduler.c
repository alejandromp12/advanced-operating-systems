#include "include/scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int _totalBaseTickets;
int *_pTickets;


void initializeScheduler(scheduler *pScheduler, OperationModeEnum mode, int totalTickets, int *pTickets)
{
	//sanity check
	if (pScheduler != NULL)
	{
		printf("totalTickets: %i, operationMode: %i\n", totalTickets, mode);
		pScheduler->mode = mode;
		pScheduler->pNextWorker = NULL;
		pScheduler->pPrevWorker = NULL;
		_totalBaseTickets = totalTickets;
		_pTickets = pTickets;
	}
}


static int getRandomTicket()
{
	// TODO
	//compute effective tickets based on
	//ticket_boost or frac. of quanta used;
	//convert effective tickets to base tickets
	//based on user's exchange rate;
	//use threshold if process is nice'd;
	//update our running count of base tickets;

	int attemps = 0;
	int randomTicket = -1;

	//sanity check
	if (_pTickets == NULL)
	{
		printf("Error, getRandomTicket(...) detected a Null pointer.\n");
		return randomTicket;
	}

	// Intializes random number generator
	time_t t;
	srand((unsigned)time(&t));

	while ((randomTicket == -1) && (attemps < _totalBaseTickets))
	{
		// take some random position from the list (0 to _totalBaseTickets)
		const int ticket = rand() % _totalBaseTickets;
		randomTicket = _pTickets[ticket];
		++attemps;
	}

	printf("Random ticket: %i\n", randomTicket);

	return randomTicket;
}


LotteryResultEnum lotteryChooseNextWorker(scheduler *pScheduler, thread *pWorkers, int workers)
{
	LotteryResultEnum processResult = ERROR;

	//sanity check
	if ((pScheduler == NULL) || (pWorkers == NULL))
	{
		printf("Error, lotteryChooseNextWorker(...) detected a Null pointer.\n");
		return processResult;
	}

	int randomTicket = getRandomTicket();
	if (randomTicket == -1)
	{
		printf("Lottery finished.\n");
		return processResult;
	}

	for (int i = 0; i < workers; i++)
	{
		if (&pWorkers[i] != NULL)
		{
			int totalWorkerTickets = (&pWorkers[i])->totalTickets;
			for (int j = 0; j < totalWorkerTickets; j++)
			{
				// check if we have a winner
				if ((&pWorkers[i])->pTickets[j] == randomTicket)
				{
					printf("ThreadId %i won the lottery, with the number %i.\n", (&pWorkers[i])->threadId, (&pWorkers[i])->pTickets[j]);
					processResult = NO_ERROR;
					pScheduler->pNextWorker = &pWorkers[i];

					return processResult;
				}
			}
		}
	}

	return processResult;
}


void removeTickets(int *pTicketsToRemove, int numTicketsToRemove)
{
	//sanity check
	if ((pTicketsToRemove == NULL) || (_pTickets == NULL))
	{
		printf("Error, removeTickets(...) detected a Null pointer.\n");
		return;
	}

	for (int i = 0; i < _totalBaseTickets; i++)
	{
		for (int j = 0; j < numTicketsToRemove; j++)
		{
			if (_pTickets[i] == pTicketsToRemove[j])
			{
				// invalidate ticket
				_pTickets[i] = -1;
			}
		}
	}
}