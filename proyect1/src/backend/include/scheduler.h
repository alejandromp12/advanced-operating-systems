#ifndef SCHEDULER_FILE
#define SCHEDULER_FILE

#include <setjmp.h>
#include <stdio.h>
#include "threadsManager.h"

typedef enum
{
	SCHEDULER_NO_ERROR = 0,
	SCHEDULER_ERROR = -1,
	LOTTERY_FINISHED = -2,
} SchedulerResultEnum; ///<

typedef enum
{
	EXPROPRIATED_MODE = 0,
	NON_EXPROPRIATED_MODE = 1,
	INVALID_MODE = 2
} OperationModeEnum; ///<

typedef enum
{
	VALID_TICKET = 0,
	INVALID_TICKET = 1
} LotteryTicketEnum; ///<

typedef struct
{
	OperationModeEnum mode;
	thread *pNextWorker;
	int numWorkers;
} scheduler; ///<

//
int initializeScheduler(scheduler *pScheduler, OperationModeEnum mode, int *pTickets, int totalBaseTickets, int numWorkers);

//
int lotteryChooseNextWorker(scheduler *pScheduler, thread *pWorkers, int workers, int *pTickets);

//
int invalidateTickets(int *pTicketsToRemove, int numTicketsToRemove, int *pTickets);

//
int validateTickets(int *pTicketsToAdd, int numTicketsToAdd, int *pTickets);

//
int haveValidTickets(int *pTickets);

#endif
