#include "include/earliestDeadlineFirstScheduler.h"
#include <math.h>

int populateEDFProcessInfo(earliestDeadlineFirst *pEDF, int numProcesses, int executionTime[], int deadline[], int remainTime[])
{
	if (!pEDF)
	{
		printf("Error, NULL ptr in populateEDFProcessInfo()\n");
		return ERROR;
	}

	pEDF->numProcesses = numProcesses;
	for (int i = 0; i < numProcesses; i++)
	{
		printf("Process %d: ", i + 1);
		pEDF->executionTime[i] = executionTime[i];
		printf("==> Execution time: %d ", pEDF->executionTime[i]);
		pEDF->remainTime[i] = executionTime[i];
		pEDF->deadline[i] = deadline[i];
		printf("==> Deadline: %d\n", pEDF->deadline[i]);
		pEDF->remainDeadline[i] = deadline[i];
	}

	return NO_ERROR;
}


int earliestDeadlineFirstScheduler(earliestDeadlineFirst *pEDF, int time)
{
	if (!pEDF)
	{
		printf("Error, NULL ptr in runEarliestDeadlineFirstScheduler()\n");
		return ERROR;
	}

	float utilization = 0;
	for (int i = 0; i < pEDF->numProcesses; i++)
	{
		utilization += ((float)pEDF->executionTime[i])/pEDF->deadline[i];
	}

	if (utilization > 1)
	{
		printf("Given problem is not schedulable.\n");
	}

	// tracking variables
	int processes[pEDF->numProcesses];
	int isReady[pEDF->numProcesses];
	for (int i = 0; i < pEDF->numProcesses; i++)
	{
		isReady[i] = 1;
		processes[i] = i + 1; 
	}

	int maxDeadline = pEDF->deadline[0];
	for (int i = 1; i < pEDF->numProcesses; i++)
	{
		if (pEDF->deadline[i] > maxDeadline)
		{
			maxDeadline = pEDF->deadline[i];
		}
	}

	// sort deadlines, execution times, and process
	int temp;
	for (int i = 0; i < pEDF->numProcesses; i++)
	{
		for (int j = i + 1; j < pEDF->numProcesses; j++)
		{	
			if (pEDF->deadline[j] < pEDF->deadline[i])
			{
				// sort execution times
				temp = pEDF->executionTime[j];
				pEDF->executionTime[j] = pEDF->executionTime[i];
				pEDF->executionTime[i] = temp;

				// sort deadlines
				temp = pEDF->deadline[j];
				pEDF->deadline[j] = pEDF->deadline[i];
				pEDF->deadline[i] = temp;

				// sort processes list
				temp = processes[j];
				processes[j] = processes[i];
				processes[i] = temp;
			}
		}
	}

	// update remain time and deadlines
	for(int i = 0; i < pEDF->numProcesses; i++)
	{
		pEDF->remainTime[i] = pEDF->executionTime[i];
		pEDF->remainDeadline[i] = pEDF->deadline[i];
	}

	int currentProcess = 0;
	int minDeadline;
	int processListEDF[time];
	for (int t = 0; t < time; t++)
	{
		if (currentProcess != -1)
		{		
			--pEDF->executionTime[currentProcess];
			processListEDF[t] = processes[currentProcess];
		}	
		else
		{
			processListEDF[t] = 0;
		}

		for (int i = 0; i< pEDF->numProcesses; i++)
		{
			--pEDF->deadline[i];
			if ((pEDF->executionTime[i] == 0) && isReady[i])
			{
				pEDF->deadline[i] += pEDF->remainDeadline[i];
				isReady[i] = 0;
			}

			if ((pEDF->deadline[i] <= pEDF->remainDeadline[i]) && (isReady[i] == 0))
			{
				pEDF->executionTime[i] = pEDF->remainTime[i];
				isReady[i] = 0;
			}
		}
	
		minDeadline = maxDeadline;		
		currentProcess = -1;
		for (int i = 0; i < pEDF->numProcesses; i++)
		{
			if ((pEDF->deadline[i] <= minDeadline) && (pEDF->executionTime[i] > 0))
			{				
				currentProcess = i;
				minDeadline = pEDF->deadline[i];
			}
		}
	}

	printSchedule(processListEDF, time, pEDF->numProcesses, EARLIEST_DEADLINE_FIRST);

	return NO_ERROR;
}