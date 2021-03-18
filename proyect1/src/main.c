#include <stdio.h>
#include <stdlib.h>
#include "backend/include/threadsManager.h"
#include "backend/include/scheduler.h"

int main(int argc, char *pArgv[])
{
	int numWorkers = 5;
	int tickets[7] = {9, 50, 3, 1, 4, 0, 2};
    thread *pWorker = (thread*)malloc(numWorkers * sizeof(thread));
    for (int i = 0; i < numWorkers; i++)
    {
	populateWorker(&pWorker[i], &tickets, 7, 3, 1);
    }

    free(pWorker);
	return 0;
}

