#ifndef THREADS_MANAGER_FILE
#define THREADS_MANAGER_FILE

#include <setjmp.h>
#include <stdio.h>

#define UNIT_OF_WORK 50

typedef struct
{
	int *pTickets;
	int totalTickets;
	int startTerm;
	int workLoad; // total
	int workLoadProgress; // current
	int quantum;
	int piApprox; // PI value
	int threadId;
	sigjmp_buf sigjmpBuf;
} thread; ///<


thread *WORKER_LIST; // threads 

double TOTAL_PI; //PI Accumulated 
///
void populateWorker(thread *pWorker, int *tickets, int totalTickets, int startTerm, int workLoad, int quantum, int threadId);

///
int updateWorkLoad(thread *pWorker, int newWorkLoad);

///
void sleepWorker(thread *pWorker);

///
void wakeupWorker(thread *pWorker);

///
void piCalculate(thread *pWorker);

#endif
