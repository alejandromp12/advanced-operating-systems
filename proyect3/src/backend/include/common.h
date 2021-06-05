#ifndef COMMON_FILE
#define COMMON_FILE

#include <stdio.h>


#define MAX_PROCESS 6

typedef enum
{
	ERROR = -1,
	NO_ERROR = 1
} Result;

typedef struct
{
	int numProcesses;
	int executionTime[MAX_PROCESS];
	int period[MAX_PROCESS];
	int remainTime[MAX_PROCESS];
} rateMonotonicGui;

//calculating the observation time for scheduling timeline
int getObservationTime(int period[]);

//print scheduling sequence
void printSchedule(int processList[], int cycles, int numProcesses);

#endif // COMMON_FILE