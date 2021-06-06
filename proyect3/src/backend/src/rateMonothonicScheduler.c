#include "include/rateMonothonicScheduler.h"
#include <math.h>
#include <stdlib.h>

//collecting details of processes
int populateRMProcessInfo(rateMonothonic *pRM, int numProcesses, int executionTime[], int period[], int remainTime[])
{
	if (!pRM)
	{
		printf("Error, NULL ptr in populateRMProcessInfo()\n");
		return ERROR;
	}

	pRM->numProcesses = numProcesses;
	pRM->executionTime = (int*)malloc(numProcesses * sizeof(int));
	pRM->remainTime = (int*)malloc(numProcesses * sizeof(int));
	pRM->period = (int*)malloc(numProcesses * sizeof(int));
	pRM->remainPeriod = (int*)malloc(numProcesses * sizeof(int));
	for (int i = 0; i < numProcesses; i++)
	{
		printf("Process %d: ", i + 1);
		pRM->executionTime[i] = executionTime[i];
		printf("==> Execution time: %d ", pRM->executionTime[i]);
		pRM->remainTime[i] = executionTime[i];
		pRM->period[i] = period[i];
		printf("==> Period: %d\n", pRM->period[i]);
		pRM->remainPeriod[i] = period[i];
	}

	return NO_ERROR;
}


int rateMonothonicScheduler(rateMonothonic *pRM, int time)
{
	if (!pRM)
	{
		printf("Error, NULL ptr in runRateMonothonicScheduler()\n");
		return ERROR;
	}

	float utilization = 0;
	for (int i = 0; i < pRM->numProcesses; i++)
	{
		utilization += ((float)pRM->executionTime[i])/pRM->period[i];
	}

	int n = pRM->numProcesses;
	if (utilization > n * (pow(2, 1.0/n) -1))
	{
		printf("Given problem is not schedulable.\n");
	}

	int processListRM[time];
	for (int i = 0; i < time; i++)
	{
		processListRM[i] = -1;
	}

	int tmpPeriod;
	int nextProcess = 0;
	int stopCond = 0;
	for (int t = 0; (t < time) && !stopCond; t++)
	{
		// logic to get the next process, in order to set the number in the array
		tmpPeriod = 100;
		for (int j = 0; j < pRM->numProcesses; j++)
		{
			if ((pRM->remainTime[j] > 0) && (pRM->period[j] < tmpPeriod))
			{
				tmpPeriod = pRM->period[j];
				nextProcess = j;
			}
		}

		// knowing the next process which could be 0, then we can assign it to the list
		// and reduce the remain
		if (pRM->remainTime[nextProcess] > 0)
		{
			processListRM[t] = nextProcess + 1; // +1 for catering 0 array index.
			pRM->remainTime[nextProcess] -= 1;
		}

		for (int k = 0; k < pRM->numProcesses; k++)
		{
			--pRM->remainPeriod[k];
			if ((pRM->remainTime[k] > pRM->remainPeriod[k]) && (pRM->remainPeriod[k] == 0))
			{
				printf("Deadline lost at process: %d\n", k + 1);
				stopCond = 1;
				break;
			}
			else if ((t + 1) % pRM->period[k] == 0)
			{
				pRM->remainPeriod[k] = pRM->period[k];
				pRM->remainTime[k] = pRM->executionTime[k];
				nextProcess = k;
			}
		}
	}

	printSchedule(processListRM, time, pRM->numProcesses, RATE_MONOTHONIC);

	return NO_ERROR;
}