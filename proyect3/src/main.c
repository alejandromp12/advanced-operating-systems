#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/time.h>

#include "backend/include/beamer.h"
#include "backend/include/common.h"
#include "backend/include/rateMonothonicScheduler.h"
#include "backend/include/earliestDeadlineFirstScheduler.h"
#include "backend/include/leastLaxityFirstScheduler.h"
//#include "frontend/include/gui.h"


rateMonotonicGui *_pRateMonotonicGui;

// sincronizes GUI elements
void getDataFromGUI()
{
	// dummy by the moment
	if (!_pRateMonotonicGui)
	{
		printf("Error, NULL ptr in getDataFromGUI()\n");
		return;
	}

	_pRateMonotonicGui->numProcesses = MAX_PROCESS;
	for (int i = 0; i < MAX_PROCESS; i++)
	{
		_pRateMonotonicGui->executionTime[i] = (rand() % 5) + 1;
		_pRateMonotonicGui->remainTime[i] = _pRateMonotonicGui->executionTime[i];
		_pRateMonotonicGui->period[i] = (rand() % 20) + 1;
	}
}

int main(int argc, char *argv[])
{
	// sanity check
	if (!argv)
	{
		printf("Error: argv is NULL.\n");
		return 1;
	}

	// Initializes time for random number generator
	time_t t;
	srand((unsigned)time(&t));


	_pRateMonotonicGui = (rateMonotonicGui*)malloc(sizeof(rateMonotonicGui));
	getDataFromGUI();

	//void (*ptr)() = &getDataFromGUI;
	//_ptrUpdateGUI = ptr;

	//runGUI(argc, argv, bufferId);

	rateMonotonic *pRateMonotonic = (rateMonotonic*)malloc(sizeof(rateMonotonic));
	if (!pRateMonotonic)
	{
		printf("Error: pRateMonotonic is NULL.\n");
		return 1;
	}

	if (populateRMProcessInfo(pRateMonotonic, _pRateMonotonicGui) == ERROR)
	{
		printf("Error: while running populateRMProcessInfo().\n");
		return 1;
	}

	int observationTime = getObservationTime(pRateMonotonic->period);
	if (observationTime == ERROR)
	{
		printf("Error: while running getObservationTime().\n");
		return 1;
	}

	if (runRateMonotonicScheduler(pRateMonotonic, observationTime) == ERROR)
	{
		printf("Error: while running rateMonotonic algorithm.\n");
		return 1;
	}

	return 0;
}
