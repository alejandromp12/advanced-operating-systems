#ifndef THREADS_MANAGER_FILE
#define THREADS_MANAGER_FILE

#include <setjmp.h>
#include <stdio.h>

#define UNIT_OF_WORK 50

typedef struct
{
	int *tickets;
	int totalTickets;
	int startTerm;
	int workLoad; // total
	int workLoadProgress; // current
	int quantum;
	int piApprox; // PI value
	sigjmp_buf sigjmpBuf;
} thread; ///<


thread *WORKER_LIST; // threads 

double TOTAL_PI = 0.0; //PI Accumulated 
///
void populateWorker(thread *pWorker, int *tickets, int totalTickets, int startTerm, int workLoad, int quantum);

///
int updateWorkLoad(thread *pWorker, int newWorkLoad);

///
void sleepWorker(thread *pWorker);

///
void wakeupWorker(thread *pWorker);

///
void piCalculate(thread *pWorker);

#endif