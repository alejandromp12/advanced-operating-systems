#ifndef THREADS_MANAGER_FILE
#define THREADS_MANAGER_FILE

#include <setjmp.h>
#include <stdio.h>

typedef struct
{
	int *pTickets;
	int totalTickets;
	int workLoad; // total
	int workLoadProgress; // current
	int quantum;
	int piApprox; // PI value
	int threadId;
	sigjmp_buf environment;
} thread; ///<


typedef enum
{
	THREAD_NO_ERROR = 0,
	THREAD_ERROR = -1,
} ThreadsManagerResultEnum; ///<

///
int populateWorker(thread *pWorker, int *pTickets, int totalTickets, int workLoad, int quantum, int threadId, sigjmp_buf environment);

///
int updateWorkLoad(thread *pWorker, int newWorkLoad);

#endif
