#include "include/common.h"
#include "include/beamer.h"

//get maximum of 2 numbers
int getMax(int *timeUnit, int size)
{
	int max = timeUnit[0];
	for (int i = 0; (i < size) && ((i + 1) < size); i++)
	{
		max = (max >= timeUnit[i + 1]) ? max : timeUnit[i + 1];
	}

	return max;
}

// Utility function to find
// GCD of 'a' and 'b'
int gcd(int a, int b)
{
	if (b == 0)
	{
		return a;
	}

	return gcd(b, a % b);
}


//calculating the observation time for scheduling timeline
int getObservationTime(int *timeUnit, int size)
{
	static int MAX_TIMELINE = 25;
	int lmc = timeUnit[0];
	for (int i = 1; i < size; i++)
	{
		lmc = (timeUnit[i] * lmc) / gcd(timeUnit[i], lmc);
	}
	return (lmc <= MAX_TIMELINE) ? lmc : MAX_TIMELINE;

	///return getMax(timeUnit, size);
}

char *getAlgorithmStr(RealTimeSchedulingAlgorithm algorithm)
{
	switch (algorithm)
	{
		case RATE_MONOTHONIC:
			return "Rate Monothonic";
		case EARLIEST_DEADLINE_FIRST:
			return "Earliest Deadline First";
		case LEAST_LAXITY_FIRST:
			return "Least Laxity First";
		default:
			return "Unknown";
	}
}


void printPresentation(int processList[], int cycles, int numProcesses, int deadline[], int deadProcess, int t_deadProcess, int executionTime[] ,RealTimeSchedulingAlgorithm algorithm)
{	
	char *algorithm_title;
	if(algorithm == RATE_MONOTHONIC)
	{
		algorithm_title = "Rate Monothonic (RM)";
	}
	else if(algorithm == EARLIEST_DEADLINE_FIRST)
	{
		algorithm_title = "Earliest Deadline First (EDF)";
	}
	else
	{
		algorithm_title = "Least Laxity First (LLF)";
	}
	simulationStep(algorithm_title, processList, cycles, numProcesses, deadline, deadProcess, t_deadProcess, executionTime);
}

//print scheduling sequence
void printSchedule(int processList[], int cycles, int numProcesses, RealTimeSchedulingAlgorithm algorithm)
{
	printf("Scheduling: %s\n", getAlgorithmStr(algorithm));
	printf("Time: ");
	for (int i = 0; i < cycles; i++)
	{
		if (i < 10)
		{
			printf("| 0%i ", i);
		}
		else
		{
			printf("| %d ", i);
		}
	}
	printf("|\n");

	for (int i = 0; i < numProcesses; i++)
	{
		printf("P[%d]: ", i + 1);
		for (int j = 0; j < cycles; j++)
		{
			if (processList[j] == i + 1)
			{
				printf("|####");
			}
			else
			{
				printf("|    ");
			}
		}
		printf("|\n");
	}
}