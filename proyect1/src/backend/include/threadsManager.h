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
	sigjmp_buf environment;
} thread; ///<

thread *WORKER_LIST; // threads 

double TOTAL_PI; //PI Accumulated 

typedef enum
{
	THREAD_NO_ERROR = 0,
	THREAD_ERROR = -1,
} ThreadsManagerResultEnum; ///<

///
int populateWorker(thread *pWorker, int *pTickets, int totalTickets, int startTerm, int workLoad, int quantum, int threadId, sigjmp_buf environment);

///
int getWorkLoadProgressInPercentage(thread *pWorker);

///
void piCalculate(thread *pWorker, int isNonExpropiated);

#endif
