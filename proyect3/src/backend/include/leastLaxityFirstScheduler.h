#ifndef LEAST_LAXITY_FIRST_SCHEDULER_FILE
#define LEAST_LAXITY_FIRST_SCHEDULER_FILE

#include <stdio.h>
#include "beamer.h"
#include "common.h"

typedef struct
{
	int numProcesses;
	int *executionTime;
	int *deadline;
	int *remainDeadline;
	int *remainTime;
	int *laxity;
	int *currentLaxity;
} leastLaxityFirst;

int populateLLFProcessInfo(leastLaxityFirst *pLLF, int numProcesses, int executionTime[], int deadline[], int remainTime[]);

int leastLaxityFirstScheduler(leastLaxityFirst *pLLF, int time, int deadLine[], int executionTime[]);

#endif // LEAST_LAXITY_FIRST_SCHEDULER_FILE