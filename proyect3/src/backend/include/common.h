#ifndef COMMON_FILE
#define COMMON_FILE

#include <stdio.h>


#define MAX_PROCESS 6
#define MAX_TIME_UNITS 7

typedef enum
{
	ERROR = -1,
	NO_ERROR = 1
} Result;

typedef enum
{
	RATE_MONOTHONIC = 0,
	EARLIEST_DEADLINE_FIRST = 1,
	LEAST_LAXITY_FIRST = 2,
} RealTimeSchedulingAlgorithm;

//calculating the observation time for scheduling timeline
int getObservationTime(int *timeUnit, int size);

//print scheduling sequence
void printSchedule(int processList[], int cycles, int numProcesses, RealTimeSchedulingAlgorithm algorithm);

//print scheduling sequence in 
void printPresentation(int processList[], int cycles, int numProcesses, int deadline[], int deadProcess, int t_deadProcess, int executionTime [], RealTimeSchedulingAlgorithm algorithm);

#endif // COMMON_FILE