#include "include/scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int _totalBaseTickets;


static int initializeTickets(int *pTickets)
{
	if (pTickets == NULL)
	{
		printf("Error, initializeTickets(...) detected a Null pointer.\n");
		return SCHEDULER_ERROR;
	}

	for (int i = 0; i < _totalBaseTickets; i++)
	{
		pTickets[i] = INVALID_TICKET;
	}

	return SCHEDULER_NO_ERROR;
}


int initializeScheduler(scheduler *pScheduler, OperationModeEnum mode, int *pTickets, int totalBaseTickets, sigjmp_buf environment)
{
	//sanity check
	if (pScheduler == NULL)
	{
		printf("Error, initializeScheduler(...) detected a Null pointer.\n");
		return SCHEDULER_ERROR;
	}

	printf("totalBaseTickets: %i, operationMode: %i\n", totalBaseTickets, mode);
	pScheduler->mode = mode;
	pScheduler->pNextWorker = NULL;
	pScheduler->pPrevWorker = NULL;
	_totalBaseTickets = totalBaseTickets;
	memcpy(&(pScheduler->environment), &environment, sizeof(sigjmp_buf));
	return initializeTickets(pTickets);
}


static int getRandomTicket(int *pTickets)
{
	// TODO
	//compute effective tickets based on
	//ticket_boost or frac. of quanta used;
	//convert effective tickets to base tickets
	//based on user's exchange rate;
	//use threshold if process is nice'd;
	//update our running count of base tickets;

	int randomTicket = SCHEDULER_ERROR;

	//sanity check
	if (pTickets == NULL)
	{
		printf("Error, getRandomTicket(...) detected a Null pointer.\n");
		return randomTicket;
	}

	// take some random position from the list (0 to _totalBaseTickets)
	int ticket = rand() % _totalBaseTickets;
	if (pTickets[ticket] == VALID_TICKET)
	{
		randomTicket = ticket;
	}

	printf("Random ticket: %i\n", randomTicket);

	return randomTicket;
}


int lotteryChooseNextWorker(scheduler *pScheduler, thread *pWorkers, int workers, int *pTickets)
{
	//sanity check
	if ((pScheduler == NULL) || (pWorkers == NULL))
	{
		printf("Error, lotteryChooseNextWorker(...) detected a Null pointer.\n");
		return SCHEDULER_ERROR;
	}

	int winner = 0;
	const int randomTicket = getRandomTicket(pTickets);
	if (randomTicket == SCHEDULER_ERROR)
	{
		return SCHEDULER_ERROR;
	}

	// remove ticket from base to avoid get it again
	pTickets[randomTicket] = INVALID_TICKET;

	for (int i = 0; i < workers; i++)
	{
		if (&pWorkers[i] != NULL)
		{
			if (winner == 1)
			{
				break;
			}

			int totalWorkerTickets = (&pWorkers[i])->totalTickets;
			for (int j = 0; j < totalWorkerTickets; j++)
			{
				if (winner == 1)
				{
					break;
				}

				// check if we have a winner
				if ((&pWorkers[i])->pTickets[j] == randomTicket)
				{
					// save the current worker as the prev one to keep a tracking
					pScheduler->pPrevWorker = pScheduler->pNextWorker;
					printf("ThreadId %i won the lottery, with the number %i.\n", (&pWorkers[i])->threadId, (&pWorkers[i])->pTickets[j]);
					pScheduler->pNextWorker = &pWorkers[i];
					winner = 1;
				}
			}
		}
	}

	return SCHEDULER_NO_ERROR;
}


int invalidateTickets(int *pTicketsToRemove, int numTicketsToRemove, int *pTickets)
{
	//sanity check
	if ((pTicketsToRemove == NULL) || (pTickets == NULL))
	{
		printf("Error, invalidateTickets(...) detected a Null pointer.\n");
		return SCHEDULER_ERROR;
	}

	int ticketNum;
	for (int i = 0; i < _totalBaseTickets; i++)
	{
		for (int j = 0; j < numTicketsToRemove; j++)
		{
			// invalidate ticket
			ticketNum = pTicketsToRemove[j];
			pTickets[ticketNum] = INVALID_TICKET;
		}
	}

	return SCHEDULER_NO_ERROR;
}


int validateTickets(int *pTicketsToAdd, int numTicketsToAdd, int *pTickets)
{
	//sanity check
	if ((pTicketsToAdd == NULL) || (pTickets == NULL))
	{
		printf("Error, validateTickets(...) detected a Null pointer.\n");
		return SCHEDULER_ERROR;
	}

	int ticketNum;
	for (int i = 0; i < _totalBaseTickets; i++)
	{
		for (int j = 0; j < numTicketsToAdd; j++)
		{
			// invalidate ticket
			ticketNum = pTicketsToAdd[j];
			pTickets[ticketNum] = VALID_TICKET;
		}
	}

	return SCHEDULER_NO_ERROR;
}


int haveValidTickets(int *pTickets)
{
	int numValidTickets = 0;

	if (pTickets == NULL)
	{
		printf("Error, initializeTickets(...) detected a Null pointer.\n");
		return numValidTickets;
	}

	for (int i = 0; i < _totalBaseTickets; i++)
	{
		if (pTickets[i] == VALID_TICKET)
		{
			++numValidTickets;
		}
	}

	return numValidTickets;
}


static void cpuHandler(thread *pWorker, scheduler *pScheduler)
{
	//sanity check
	if ((pWorker == NULL) || (pScheduler == NULL))
	{
		printf("Error, cpuHandler(...) detected a Null pointer.\n");
		return;
	}

	switch (pScheduler->mode)
	{
		case EXPROPRIATED_MODE:
		{
			printf("Expropiated mode.\n");
			clock_t timeElapsed;
			while (timeElapsed < pWorker->quantum)
			{
				timeElapsed = clock() - timeElapsed;
			}
			break;
		}

		case NON_EXPROPRIATED_MODE:
		{
			printf("Non Expropiated mode.\n");
			break;
		}

		case INVALID_MODE:
		default:
			printf("Error, Unknown mode.\n");
			break;
	}
}


void prepareWorkersEnvironment(thread *pWorkers, scheduler *pScheduler, int numWorkers)
{
	//sanity check
	if (pWorkers == NULL)
	{
		printf("Error, sleepWorker(...) detected a Null pointer.\n");
		return;
	}

	for (int i = 0; i < numWorkers; i++)
	{
		if (sigsetjmp((&pWorkers[i])->environment, (&pWorkers[i])->threadId) == 0)
		{
			printf("Worker %i just went to sleep.\n", (&pWorkers[i])->threadId);
		}
		else
		{
			printf("DO THREAD WORK.\n");
			cpuHandler(&pWorkers[i], pScheduler);
			printf("Scheduler just woke up.\n");
			siglongjmp(pScheduler->environment, 1);
		}
	}
}
