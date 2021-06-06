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
#include "frontend/include/gui.h"

int _executionTimeRM[MAX_PROCESS];
int _remainTimeRM[MAX_PROCESS];
int _periodRM[MAX_PROCESS];
int _numProcessesRM;

int _executionTimeEDF[MAX_PROCESS];
int _remainTimeEDF[MAX_PROCESS];
int _deadlineEDF[MAX_PROCESS];
int _numProcessesEDF;

rateMonothonic *_pRateMonothonic;
earliestDeadlineFirst *_pEarliestDeadlineFirst;

// sincronizes GUI elements
void getDataFromGUI()
{
	// Initializes time for random number generator: dummy by the moment
	time_t t;
	srand((unsigned)time(&t));

	// dummy by the moment
	_numProcessesRM = MAX_PROCESS;
	int tmp;
	for (int i = 0; i < _numProcessesRM; i++)
	{
		_executionTimeRM[i] = (rand() % MAX_TIME_UNITS) + 1;
		_remainTimeRM[i] = _executionTimeRM[i];
		tmp = (rand() % MAX_TIME_UNITS) + _executionTimeRM[i];
		_periodRM[i] = (tmp <= MAX_TIME_UNITS) ? tmp : MAX_TIME_UNITS;
	}

	// dummy by the moment
	_numProcessesEDF = MAX_PROCESS;
	for (int i = 0; i < _numProcessesEDF; i++)
	{
		_executionTimeEDF[i] = (rand() % MAX_TIME_UNITS) + 1;
		_remainTimeEDF[i] = _executionTimeEDF[i];
		tmp = (rand() % MAX_TIME_UNITS) + _executionTimeEDF[i];
		_deadlineEDF[i] = (tmp <= MAX_TIME_UNITS) ? tmp : MAX_TIME_UNITS;
	}

	exit_app();
}


void runRateMonothonicScheduler()
{
	if (populateRMProcessInfo(_pRateMonothonic, _numProcessesRM, _executionTimeRM, _periodRM, _remainTimeRM) == ERROR)
	{
		printf("Error: while running populateRMProcessInfo().\n");
		_pRateMonothonic = NULL;
	}

	if (!_pRateMonothonic)
	{
		printf("Error: _pRateMonothonic is NULL.\n");
		exit(1);
	}

	int observationTimeRM = getObservationTime(_pRateMonothonic->period, _pRateMonothonic->numProcesses);
	if (observationTimeRM == ERROR)
	{
		printf("Error: while running getObservationTime().\n");
		exit(1);
	}

	if (rateMonothonicScheduler(_pRateMonothonic, observationTimeRM) == ERROR)
	{
		printf("Error: while running RateMonothonic algorithm.\n");
		exit(1);
	}

	// clean up section
	if (_pRateMonothonic != NULL)
	{
		free(_pRateMonothonic);
	}
}

	//runGUI(argc, argv, bufferId);

void runEarliestDeadlineFirstScheduler()
{
	if (populateEDFProcessInfo(_pEarliestDeadlineFirst, _numProcessesEDF, _executionTimeEDF, _deadlineEDF, _remainTimeEDF) == ERROR)
	{
		printf("Error: while running populateEDFProcessInfo().\n");
		_pEarliestDeadlineFirst = NULL;
	}

	if (!_pEarliestDeadlineFirst)
	{
		printf("Error: _pEarliestDeadlineFirst is NULL.\n");
		exit(1);
	}

	int observationTimeEDF = getObservationTime(_pEarliestDeadlineFirst->deadline, _pEarliestDeadlineFirst->numProcesses);
	if (observationTimeEDF == ERROR)
	{
		printf("Error: while running getObservationTime().\n");
		exit(1);
	}

	if (earliestDeadlineFirstScheduler(_pEarliestDeadlineFirst, observationTimeEDF) == ERROR)
	{
		printf("Error: while running EarliestDeadlineFirst algorithm.\n");
		exit(1);
	}

		// clean up section
	if (_pEarliestDeadlineFirst != NULL)
	{
		free(_pEarliestDeadlineFirst);
	}
}


int main(int argc, char *argv[])
{
	// sanity check
	if (!argv)
	{
		printf("Error: argv is NULL.\n");
		exit(1);
	}

	_pRateMonothonic = (rateMonothonic*)malloc(sizeof(rateMonothonic));
	_pEarliestDeadlineFirst = (earliestDeadlineFirst*)malloc(sizeof(earliestDeadlineFirst));
	
	void (*ptr)() = &getDataFromGUI;
	_ptrGetFromGUI = ptr;
	runGUI(argc, argv);


	//getDataFromGUI();
	runRateMonothonicScheduler();
	//runEarliestDeadlineFirstScheduler();

	return 0;
}
