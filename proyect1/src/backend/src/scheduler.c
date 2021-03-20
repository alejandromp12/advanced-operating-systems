#include "include/scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int _totalBaseTickets;


static void initializeTickets(int *pTickets)
{
	if (pTickets == NULL)
	{
		printf("Error, initializeTickets(...) detected a Null pointer.\n");
	}

	for (int i = 0; i < _totalBaseTickets; i++)
	{
		pTickets[i] = INVALID_TICKET;
	}
}


void initializeScheduler(scheduler *pScheduler, OperationModeEnum mode, int *pTickets, int totalBaseTickets)
{
	//sanity check
	if (pScheduler != NULL)
	{
		printf("totalBaseTickets: %i, operationMode: %i\n", totalBaseTickets, mode);
		pScheduler->mode = mode;
		pScheduler->pNextWorker = NULL;
		pScheduler->pPrevWorker = NULL;
		_totalBaseTickets = totalBaseTickets;
		initializeTickets(pTickets);
	}
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

	int attemps = 0;
	int randomTicket = -1;

	//sanity check
	if (pTickets == NULL)
	{
		printf("Error, getRandomTicket(...) detected a Null pointer.\n");
		return randomTicket;
	}

	// Intializes random number generator
	time_t t;
	srand((unsigned)time(&t));

	int spinLotteryWheel = 1;
	int ticket;
	while ((spinLotteryWheel == 1) && (attemps < _totalBaseTickets))
	{
		// take some random position from the list (0 to _totalBaseTickets)
		ticket = rand() % _totalBaseTickets;
		if (pTickets[ticket] == VALID_TICKET)
		{
			// stop lottery
			spinLotteryWheel = 0;
			randomTicket = ticket;
		}

		++attemps;
	}

	printf("Random ticket: %i\n", randomTicket);

	return randomTicket;
}


LotteryResultEnum lotteryChooseNextWorker(scheduler *pScheduler, thread *pWorkers, int workers, int *pTickets)
{
	//sanity check
	if ((pScheduler == NULL) || (pWorkers == NULL))
	{
		printf("Error, lotteryChooseNextWorker(...) detected a Null pointer.\n");
		return ERROR;
	}

	int winer = 0;
	const int randomTicket = getRandomTicket(pTickets);
	for (int i = 0; i < workers; i++)
	{
		if (&pWorkers[i] != NULL)
		{
			if (winer == 1)
			{
				break;
			}

			int totalWorkerTickets = (&pWorkers[i])->totalTickets;
			for (int j = 0; j < totalWorkerTickets; j++)
			{
				if (winer == 1)
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

					winer = 1;
				}
			}
		}
	}

	return NO_ERROR;
}


void invalidateTickets(int *pTicketsToRemove, int numTicketsToRemove, int *pTickets)
{
	//sanity check
	if ((pTicketsToRemove == NULL) || (pTickets == NULL))
	{
		printf("Error, invalidateTickets(...) detected a Null pointer.\n");
		return;
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
}


void validateTickets(int *pTicketsToAdd, int numTicketsToAdd, int *pTickets)
{
	//sanity check
	if ((pTicketsToAdd == NULL) || (pTickets == NULL))
	{
		printf("Error, validateTickets(...) detected a Null pointer.\n");
		return;
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
