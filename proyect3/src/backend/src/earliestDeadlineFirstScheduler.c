#include "include/earliestDeadlineFirstScheduler.h"
#include <math.h>
#include <stdlib.h>

int populateEDFProcessInfo(earliestDeadlineFirst *pEDF, int numProcesses, int executionTime[], int deadline[], int remainTime[])
{
	if (!pEDF)
	{
		printf("Error, NULL ptr in populateEDFProcessInfo()\n");
		return ERROR;
	}

	pEDF->numProcesses = numProcesses;
	pEDF->executionTime = (int*)malloc(numProcesses * sizeof(int));
	pEDF->remainTime = (int*)malloc(numProcesses * sizeof(int));
	pEDF->deadline = (int*)malloc(numProcesses * sizeof(int));
	pEDF->remainDeadline = (int*)malloc(numProcesses * sizeof(int));
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


int earliestDeadlineFirstScheduler(earliestDeadlineFirst *pEDF, int time, int deadLine[], int executionTime[])
{
	if (!pEDF)
	{
		printf("Error, NULL ptr in earliestDeadlineFirstScheduler()\n");
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

	int deadProcess = -1;
	int t_deadProcess = -1;
	// tracking variables for ?
	int processes[pEDF->numProcesses];
	int isReady[pEDF->numProcesses];
	for (int i = 0; i < pEDF->numProcesses; i++)
	{
		isReady[i] = 1;
		processes[i] = i + 1; 
	}

	// capture the higher deadline
	int maxDeadline = pEDF->deadline[0];
	for (int i = 1; i < pEDF->numProcesses; i++)
	{
		if (pEDF->deadline[i] > maxDeadline)
		{
			maxDeadline = pEDF->deadline[i];
		}
	}

	// sort deadlines, execution times, and process from lower to higher
	int temp;
	for (int i = 0; i < pEDF->numProcesses; i++)
	{
		// next process
		for (int j = i + 1; j < pEDF->numProcesses; j++)
		{
			// only when the deadline of the next processes is lower than the current one
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

	// set default values
	int processListEDF[time];
	for(int i = 0; i < time; i++)
	{
		processListEDF[i] = -1;
	}

	int currentProcess = 0;
	int minDeadline;
	int stopCond = 0;
	for (int t = 0; (t < time) && !stopCond; t++)
	{
		if ((currentProcess != -1) && (pEDF->executionTime[currentProcess] > 0))
		{
			--pEDF->executionTime[currentProcess];
			processListEDF[t] = processes[currentProcess];
			//printf("pEDF->executionTime[%i]: %d\n", currentProcess, pEDF->executionTime[currentProcess]);
		}

		for (int i = 0; i < pEDF->numProcesses; i++)
		{
			// reduce the deadline of all the processes
			if (pEDF->deadline[i] > 0)
			{
				--pEDF->deadline[i];
			}
		}

		for (int i = 0; i < pEDF->numProcesses; i++)
		{
			//printf("=======================================\n");
			//printf("process: %d\n", processes[i]);
			//printf("pEDF->executionTime[%i]: %d\n", i, pEDF->executionTime[i]);
			//printf("pEDF->deadline[%i]: %d\n", i, pEDF->deadline[i]);
			if ((pEDF->deadline[i] == 0) && (pEDF->executionTime[i] > pEDF->deadline[i]) && isReady[i])
			{
				printf("Deadline lost at process: %d\n", processes[i]);
				deadProcess = processes[i];


				t_deadProcess = t;
				printf("-- %d --", t_deadProcess);
				stopCond = 1;
				//printf("=======================================\n");
				break;
			}
			//printf("=======================================\n");
		}

		for (int i = 0; i < pEDF->numProcesses; i++)
		{
			// only when a process already consumed its execution time
			if ((pEDF->executionTime[i] == 0) && (pEDF->deadline[i] == 0) && isReady[i])
			{
				pEDF->deadline[i] = pEDF->remainDeadline[i];
				isReady[i] = 0;
			}

			// when process is about to accomplish its deadline (it already did previously)
			if ((pEDF->deadline[i] <= pEDF->remainDeadline[i]) && (isReady[i] == 0))
			{
				pEDF->executionTime[i] = pEDF->remainTime[i];
				isReady[i] = 1;
			}
		}

		//readyCounter = 0;
		//for (int i = 0; i < pEDF->numProcesses; i++)
		//{
		//	if (isReady[i] == 0)
		//	{
		//		++readyCounter;
		//	}
		//}
		//if (pEDF->numProcesses == readyCounter)
		//{
		//	for (int i = 0; i < pEDF->numProcesses; i++)
		//	{
		//		isReady[i] = 1;
		//	}
		//}
	
		minDeadline = maxDeadline;		
		currentProcess = -1;
		for (int i = 0; i < pEDF->numProcesses; i++)
		{
			if ((pEDF->deadline[i] <= minDeadline) && (pEDF->executionTime[i] > 0) && isReady[i])
			{				
				currentProcess = i;
				minDeadline = pEDF->deadline[i];
			}
		}
	}

	printSchedule(processListEDF, time, pEDF->numProcesses, EARLIEST_DEADLINE_FIRST);
	if(_mixedSlidesFlag==0)
	{
		printPresentation(processListEDF, time, pEDF ->numProcesses, deadLine, deadProcess, t_deadProcess, executionTime,EARLIEST_DEADLINE_FIRST);
	}
	else{

		EDFData.time = time;
		EDFData.numProcess = pEDF ->numProcesses;
		EDFData.deadProcess = deadProcess;
		EDFData.t_deadProcess = t_deadProcess;
		for(int k = 0; k < time; k++)
		{
			EDFData.processList[k] = processListEDF[k];
		}

		for(int k = 0; k < pEDF ->numProcesses; k++)
		{
			EDFData.deadline[k] = deadLine[k];
			EDFData.executionTime[k] = executionTime[k];
		}
	}

	return NO_ERROR;
}