#ifndef SCHEDULER_FILE
#define SCHEDULER_FILE

#include <stdio.h>
#include "threadsManager.h"

typedef enum
{
	NO_ERROR = 0,
	ERROR = 1
} LotteryResultEnum; ///<

typedef enum
{
	EXPROPRIATED_MODE = 0,
	NON_EXPROPRIATED_MODE = 1
} OperationModeEnum; ///<

typedef struct
{
	int totalBaseTickets;
	OperationModeEnum mode;
	thread *pNextWorker;
} scheduler; ///<

//void
void initializeScheduler(scheduler *pScheduler, OperationModeEnum mode, int totalTickets);

//
LotteryResultEnum lotteryChooseNextWorker(scheduler *pScheduler, thread *pWorkers, int workers);

#endif