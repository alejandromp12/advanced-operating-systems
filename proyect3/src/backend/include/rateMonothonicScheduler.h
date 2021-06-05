#ifndef RATE_MONOTHONIC_SHCEDULER_FILE
#define RATE_MONOTHONIC_SHCEDULER_FILE

#include <stdio.h>
#include "common.h"

typedef struct
{
	int numProcesses;
	int executionTime[MAX_PROCESS];
	int period[MAX_PROCESS];
	int remainTime[MAX_PROCESS];
} rateMonotonic;

int populateRMProcessInfo(rateMonotonic *pRM, rateMonotonicGui *pRMGui);

int runRateMonotonicScheduler(rateMonotonic *pRM, int time);


#endif // RATE_MONOTHONIC_SHCEDULER_FILE