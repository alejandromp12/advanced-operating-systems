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

typedef enum
{
	VALID_TICKET = 0,
	INVALID_TICKET = 1
} LotteryTicketEnum; ///<

typedef struct
{
	OperationModeEnum mode;
	thread *pNextWorker;
	thread *pPrevWorker;
} scheduler; ///<

//
void initializeScheduler(scheduler *pScheduler, OperationModeEnum mode, int *pTickets, int totalBaseTickets);

//
LotteryResultEnum lotteryChooseNextWorker(scheduler *pScheduler, thread *pWorkers, int workers, int *pTickets);

//
void invalidateTickets(int *pTicketsToRemove, int numTicketsToRemove, int *pTickets);

//
void validateTickets(int *pTicketsToAdd, int numTicketsToAdd, int *pTickets);

//
int haveValidTickets(int *pTickets);

#endif
