#include "include/leastLaxityFirstScheduler.h"
#include <math.h>
#include <stdlib.h>

int populateLLFProcessInfo(leastLaxityFirst *pLLF, int numProcesses, int executionTime[], int deadline[], int remainTime[])
{
	if (!pLLF)
	{
		printf("Error, NULL ptr in populateLLFProcessInfo()\n");
		return ERROR;
	}

	pLLF->numProcesses = numProcesses;
	pLLF->executionTime = (int*)malloc(numProcesses * sizeof(int));
	pLLF->remainTime = (int*)malloc(numProcesses * sizeof(int));
	pLLF->deadline = (int*)malloc(numProcesses * sizeof(int));
	pLLF->laxity = (int*)malloc(numProcesses * sizeof(int));
	pLLF->currentLaxity = (int*)malloc(numProcesses * sizeof(int));
	pLLF->remainDeadline = (int*)malloc(numProcesses * sizeof(int));
	for (int i = 0; i < numProcesses; i++)
	{
		printf("Process %d: ", i + 1);
		pLLF->executionTime[i] = executionTime[i];
		printf("==> Execution time: %d ", pLLF->executionTime[i]);
		pLLF->remainTime[i] = executionTime[i];
		pLLF->deadline[i] = deadline[i];
		pLLF->remainDeadline[i] = deadline[i];
		printf("==> Deadline: %d ", pLLF->deadline[i]);
		pLLF->currentLaxity[i] = pLLF->deadline[i] - pLLF->executionTime[i];
		printf("==> Laxity: %d\n", pLLF->currentLaxity[i]);
		pLLF->laxity[i] = pLLF->currentLaxity[i];
	}

	return NO_ERROR;
}


int leastLaxityFirstScheduler(leastLaxityFirst *pLLF, int time, int deadLine[], int executionTime[])
{
	if (!pLLF)
	{
		printf("Error, NULL ptr in leastLaxityFirstScheduler()\n");
		return ERROR;
	}

	// investigate
	float utilization = 0;
	for (int i = 0; i < pLLF->numProcesses; i++)
	{
		utilization += ((float)pLLF->executionTime[i])/pLLF->deadline[i];
	}

	// investigate
	if (utilization > 1)
	{
		printf("Given problem is not schedulable.\n");
	}

	int deadProcess = -1;
	int t_deadProcess = -1;
	// tracking variables for ?
	int processes[pLLF->numProcesses];
	int isReady[pLLF->numProcesses];
	for (int i = 0; i < pLLF->numProcesses; i++)
	{
		isReady[i] = 1;
		processes[i] = i + 1; 
	}

	// capture the higher laxity
	int maxLaxity = pLLF->deadline[0] - pLLF->executionTime[0];
	int tmpLaxity;
	for (int i = 1; i < pLLF->numProcesses; i++)
	{
		tmpLaxity = pLLF->deadline[i] - pLLF->executionTime[i];
		if (tmpLaxity > maxLaxity)
		{
			maxLaxity = tmpLaxity;
		}
	}

	// sort deadlines, execution times, and process from lower to higher laxity
	int temp;
	for (int i = 0; i < pLLF->numProcesses; i++)
	{
		// next process
		for (int j = i + 1; j < pLLF->numProcesses; j++)
		{
			// only when the laxity of the next processes is lower than the current one
			if ((pLLF->deadline[j] - pLLF->executionTime[j]) < (pLLF->deadline[i] - pLLF->executionTime[i]))
			{
				// sort execution times
				temp = pLLF->executionTime[j];
				pLLF->executionTime[j] = pLLF->executionTime[i];
				pLLF->executionTime[i] = temp;

				// sort deadlines
				temp = pLLF->deadline[j];
				pLLF->deadline[j] = pLLF->deadline[i];
				pLLF->deadline[i] = temp;

				// sort processes list
				temp = processes[j];
				processes[j] = processes[i];
				processes[i] = temp;
			}
		}
	}

	// update remain time, deadlines and laxity
	for(int i = 0; i < pLLF->numProcesses; i++)
	{
		pLLF->remainTime[i] = pLLF->executionTime[i];
		pLLF->remainDeadline[i] = pLLF->deadline[i];
		pLLF->currentLaxity[i] = pLLF->deadline[i] - pLLF->executionTime[i];
		pLLF->laxity[i] = pLLF->currentLaxity[i];
	}

	// set default values
	int processListLLF[time];
	for(int i = 0; i < time; i++)
	{
		processListLLF[i] = -1;
	}

	int currentProcess = 0;
	int minLaxity;
	int stopCond = 0;
	int N;
	for (int t = 0; (t < time) && !stopCond; t++)
	{
		//for (int i = 0; i < pLLF->numProcesses; i++)
		//{
		//	printf("=======================================\n");
		//	printf("pLLF->currentLaxity[%i]: %d\n", i, pLLF->currentLaxity[i]);
		//	printf("=======================================\n");
		//}
		if ((currentProcess != -1) && (pLLF->executionTime[currentProcess] > 0))
		{
			--pLLF->executionTime[currentProcess];
			processListLLF[t] = processes[currentProcess];
			//printf("pLLF->executionTime[%i]: %d\n", currentProcess, pLLF->executionTime[currentProcess]);
		}

		for (int i = 0; i < pLLF->numProcesses; i++)
		{
			// reduce the deadline of all the processes
			if (pLLF->deadline[i] > 0)
			{
				--pLLF->remainDeadline[i];
				if ((t + 1) > pLLF->deadline[i])
				{
					N = (int)ceil((t + 1) / pLLF->deadline[i]);
					pLLF->currentLaxity[i] = (N * pLLF->deadline[i]) - (t + 1) - pLLF->executionTime[i];
				}
				else
				{
					pLLF->currentLaxity[i] = pLLF->deadline[i] - (t + 1) - pLLF->executionTime[i];
				}
			}
		}

		for (int i = 0; i < pLLF->numProcesses; i++)
		{
			//printf("=======================================\n");
			//printf("process: %d\n", processes[i]);
			//printf("pLLF->executionTime[%i]: %d\n", i, pLLF->executionTime[i]);
			//printf("pLLF->deadline[%i]: %d\n", i, pLLF->deadline[i]);
			if ((pLLF->currentLaxity[i] <= 0) && (pLLF->executionTime[i] > pLLF->remainDeadline[i]) && (pLLF->remainDeadline[i] == 0) && isReady[i])
			{
				printf("Deadline lost at process: %d\n", processes[i]);
				deadProcess = processes[i];
				t_deadProcess = t;

				stopCond = 1;
				//printf("=======================================\n");
				break;
			}
			//printf("=======================================\n");
		}


		for (int i = 0; i < pLLF->numProcesses; i++)
		{
			//printf("pLLF->currentLaxity[%i]: %d\n", i, pLLF->currentLaxity[i]);

			// only when a process already consumed its execution time
			if ((pLLF->executionTime[i] == 0) && (pLLF->remainDeadline[i] == 0) && isReady[i])
			{
				pLLF->remainDeadline[i] = pLLF->deadline[i];
				isReady[i] = 0;
			}

			// when process is about to accomplish its deadline (it already did previously)
			if ((pLLF->remainDeadline[i] <= pLLF->deadline[i]) && (isReady[i] == 0))
			{
				pLLF->executionTime[i] = pLLF->remainTime[i];
				pLLF->currentLaxity[i] = pLLF->laxity[i];
				isReady[i] = 1;
			}
		}

		// slect the process with the lower laxity
		minLaxity = maxLaxity;		
		currentProcess = -1;
		for (int i = 0; i < pLLF->numProcesses; i++)
		{
			//printf("minLaxity: %d\n",minLaxity);
			if ((pLLF->currentLaxity[i] <= minLaxity) && (pLLF->executionTime[i] > 0) && isReady[i])
			{				
				currentProcess = i;
				//printf("processes[%i]: %d\n",currentProcess, processes[currentProcess]);
				minLaxity = pLLF->currentLaxity[i];
			}
		}
	}

	printSchedule(processListLLF, time, pLLF->numProcesses, LEAST_LAXITY_FIRST);
	printPresentation(processListLLF, time, pLLF ->numProcesses, deadLine, deadProcess, t_deadProcess, executionTime,LEAST_LAXITY_FIRST);

	return NO_ERROR;
}