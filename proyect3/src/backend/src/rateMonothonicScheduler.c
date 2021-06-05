#include "include/rateMonothonicScheduler.h"
#include <math.h>

//collecting details of processes
int populateRMProcessInfo(rateMonotonic *pRM, rateMonotonicGui *pRMGui)
{
	if (!pRM || !pRMGui)
	{
		printf("Error, NULL ptr in populateRMProcessInfo()\n");
		return ERROR;
	}

	pRM->numProcesses = pRMGui->numProcesses;
	for (int i = 0; i < pRM->numProcesses; i++)
	{
		printf("Process %d:-", i + 1);
		pRM->executionTime[i] = pRMGui->executionTime[i];
		printf("==> Execution time: %d ", pRM->executionTime[i]);
		pRM->remainTime[i] = pRMGui->executionTime[i];
		pRM->period[i] = pRMGui->period[i];
		printf("==> Period: %d\n", pRM->period[i]);
	}

	return NO_ERROR;
}

int runRateMonotonicScheduler(rateMonotonic *pRM, int time)
{
	if (!pRM)
	{
		printf("Error, NULL ptr in rateMonotonic()\n");
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
	int min = 999;
	int nextProcess = 0;
	for (int i = 0; i < time; i++)
	{
		min = 1000;
		for (int j = 0; j < pRM->numProcesses; j++)
		{
			if (pRM->remainTime[j] > 0)
			{
				if (min > pRM->period[j])
				{
					min = pRM->period[j];
					nextProcess = j;
				}
			}
		}

		if (pRM->remainTime[nextProcess] > 0)
		{
			processListRM[i] = nextProcess + 1; 	// +1 for catering 0 array index.
			pRM->remainTime[nextProcess] -= 1;
		}

		for (int k = 0; k < pRM->numProcesses; k++)
		{
			if ((i + 1) % pRM->period[k] == 0)
			{
				pRM->remainTime[k] = pRM->executionTime[k];
				nextProcess = k;
			}
		}
	}

	printSchedule(processListRM, time, pRM->numProcesses);

	return NO_ERROR;
}