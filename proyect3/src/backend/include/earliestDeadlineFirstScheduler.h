#ifndef EARLIEST_DEADLINE_FIRST_SCHEDULER_FILE
#define EARLIEST_DEADLINE_FIRST_SCHEDULER_FILE

#include <stdio.h>

#include "common.h"

typedef struct
{
	int numProcesses;
	int *executionTime;
	int *deadline;
	int *remainTime;
	int *remainDeadline;
} earliestDeadlineFirst;

int populateEDFProcessInfo(earliestDeadlineFirst *pEDF, int numProcesses, int executionTime[], int deadline[], int remainTime[]);

int earliestDeadlineFirstScheduler(earliestDeadlineFirst *pEDF, int time, int deadLine[], int executionTime[]);

#endif // EARLIEST_DEADLINE_FIRST_SCHEDULER_FILE