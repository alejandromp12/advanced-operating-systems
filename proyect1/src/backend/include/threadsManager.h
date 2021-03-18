#ifndef THREADS_MANAGER_FILE
#define THREADS_MANAGER_FILE

#include <setjmp.h>
#include <stdio.h>

typedef struct
{
	int *tickets;
	int totalTickets;
	int workLoad; // total
	int workLoadProgress; // current
	int quantum;
	int piApprox; // PI value
	sigjmp_buf sigjmpBuf;
} thread; ///<

///
void populateWorker(thread *pWorker, int *tickets, int totalTickets, int workLoad, int quantum);

///
int updateWorkLoad(thread *pWorker, int newWorkLoad);

///
void sleepWorker(thread *pWorker);

///
void wakeupWorker(thread *pWorker);

#endif