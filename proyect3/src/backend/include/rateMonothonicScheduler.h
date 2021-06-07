#ifndef RATE_MONOTHONIC_SHCEDULER_FILE
#define RATE_MONOTHONIC_SHCEDULER_FILE

#include <stdio.h>
#include "beamer.h"
#include "common.h"

typedef struct
{
	int numProcesses;
	int *executionTime;
	int *period;
	int *remainTime;
	int *remainPeriod;
} rateMonothonic;

int populateRMProcessInfo(rateMonothonic *pRM, int numProcesses, int executionTime[], int period[], int remainTime[]);

int rateMonothonicScheduler(rateMonothonic *pRM, int time);


#endif // RATE_MONOTHONIC_SHCEDULER_FILE