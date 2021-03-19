#include <stdio.h>
#include <stdlib.h>
#include "backend/include/threadsManager.h"
#include "backend/include/scheduler.h"

int main(int argc, char *pArgv[])
{
	int numWorkers = 5;
	int tickets1[7] = {9, 50, 3, 1, 4, 0, 2};
	int tickets2[2] = {8, 51};
	int tickets3[6] = {10, 45, 15, 19, 31, 21};
	int tickets4[3] = {85, 99, 80};
	int tickets5[1] = {100};
	int **tickets = (int**)malloc(5 * sizeof(int*));
        for (int i = 0; i < 5; i++)
        {
		if (i == 0)
		{
			tickets[i] = tickets1;
		}
                else if (i == 1)
		{
			tickets[i] = tickets2;
		}
                else if (i == 2)
                {
                        tickets[i] = tickets3;
                }
                else if (i == 3)
                {
                        tickets[i] = tickets4;
                }
                else if (i == 4)
                {
                        tickets[i] = tickets5;
                }
        }

	thread *pWorker = (thread*)malloc(numWorkers * sizeof(thread));
	for (int i = 0; i < numWorkers; i++)
	{
		populateWorker(&pWorker[i], tickets[i], 7, 3, 1, i);
	}

	scheduler systemScheduler;
	initializeScheduler(&systemScheduler, EXPROPRIATED_MODE, 19, tickets);
	while (NO_ERROR == lotteryChooseNextWorker(&systemScheduler, pWorker, numWorkers))
	{
		// simulate that the worker has to be removed
		thread *pThread = (thread*)systemScheduler.pPrevWorker;
		if (pThread != NULL)
		{
			// remove its tickest
			removeTickets(pThread->pTickets, pThread->totalTickets);
		}
	}

	free(pWorker);
	for (int i = 0; i < 5; i++)
	{
		free(tickets[i]);
	}
	free(tickets);
	return 0;
}

