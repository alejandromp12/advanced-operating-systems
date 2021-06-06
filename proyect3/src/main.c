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

int *_executionTime;
int *_remainTime;
int *_timeLine;
int _numProcesses;

int _rmFlag = 0;
int _edfFlag = 0;
int _llfFlag = 0;
int _mixedSlidesFlag = 0;

rateMonothonic *_pRateMonothonic;
earliestDeadlineFirst *_pEarliestDeadlineFirst;
leastLaxityFirst *_pLeastLaxityFirst;

// sincronizes GUI elements
void getDataFromGUI()
{
	_rmFlag = _selectedRM;
	_edfFlag = _selectedEDF;
	_llfFlag = _selectedLLF;
	_mixedSlidesFlag = _mixedSlides;

	_numProcesses = _numTasks;
	_executionTime = (int*)malloc(_numProcesses * sizeof(int));
	_remainTime = (int*)malloc(_numProcesses * sizeof(int));
	_timeLine = (int*)malloc(_numProcesses * sizeof(int));
	for (int i = 0; i < _numProcesses; i++)
	{
		_executionTime[i] = tasksGUI[i].executionTime;
		_remainTime[i] = _executionTime[i];
		_timeLine[i] = tasksGUI[i].periodTime;
	}

	exit_app();
/*
	// Initializes time for random number generator: dummy by the moment
	time_t t;
	srand((unsigned)time(&t));

	// dummy by the moment
	_numProcesses = MAX_PROCESS;
	int tmp;
	for (int i = 0; i < _numProcesses; i++)
	{
		_executionTime[i] = (rand() % MAX_TIME_UNITS) + 1;
		_remainTime[i] = _executionTime[i];
		tmp = (rand() % MAX_TIME_UNITS) + _executionTime[i];
		_timeLine[i] = (tmp <= MAX_TIME_UNITS) ? tmp : MAX_TIME_UNITS;
	}

	if (1)
	{
		_executionTime[0] = 4;
		_executionTime[1] = 5;
		_executionTime[2] = 5;
		_executionTime[3] = 4;
		_executionTime[4] = 1;
		_executionTime[5] = 6;

		_timeLine[0] = _executionTime[0];
		_timeLine[1] = _executionTime[1];
		_timeLine[2] = _executionTime[2];
		_timeLine[3] = _executionTime[3];
		_timeLine[4] = _executionTime[4];
		_timeLine[5] = _executionTime[5];
	
		_timeLine[0] = 7;
		_timeLine[1] = 6;
		_timeLine[2] = 7;
		_timeLine[3] = 6;
		_timeLine[4] = 4;
		_timeLine[5] = 7;
	}
	else
	{
		_executionTime[0] = 6;
		_executionTime[1] = 6;
		_executionTime[2] = 5;
		_executionTime[3] = 6;
		_executionTime[4] = 2;
		_executionTime[5] = 1;

		_timeLine[0] = _executionTime[0];
		_timeLine[1] = _executionTime[1];
		_timeLine[2] = _executionTime[2];
		_timeLine[3] = _executionTime[3];
		_timeLine[4] = _executionTime[4];
		_timeLine[5] = _executionTime[5];

		_timeLine[0] = 7;
		_timeLine[1] = 6;
		_timeLine[2] = 7;
		_timeLine[3] = 7;
		_timeLine[4] = 4;
		_timeLine[5] = 1;
	}
*/
}


void runRateMonothonicScheduler()
{
	if (populateRMProcessInfo(_pRateMonothonic, _numProcesses, _executionTime, _timeLine, _remainTime) == ERROR)
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
}


void runEarliestDeadlineFirstScheduler()
{
	if (populateEDFProcessInfo(_pEarliestDeadlineFirst, _numProcesses, _executionTime, _timeLine, _remainTime) == ERROR)
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

	if (earliestDeadlineFirstScheduler(_pEarliestDeadlineFirst, observationTimeEDF, _timeLine, _executionTime) == ERROR)
	{
		printf("Error: while running EarliestDeadlineFirst algorithm.\n");
		exit(1);
	}
}


void runLeastLaxityFirstScheduler()
{
	if (populateLLFProcessInfo(_pLeastLaxityFirst, _numProcesses, _executionTime, _timeLine, _remainTime) == ERROR)
	{
		printf("Error: while running populateLLFProcessInfo().\n");
		_pLeastLaxityFirst = NULL;
	}

	if (!_pLeastLaxityFirst)
	{
		printf("Error: _pLeastLaxityFirst is NULL.\n");
		exit(1);
	}

	int observationTimeLLF = getObservationTime(_pLeastLaxityFirst->deadline, _pLeastLaxityFirst->numProcesses);
	if (observationTimeLLF == ERROR)
	{
		printf("Error: while running getObservationTime().\n");
		exit(1);
	}

	if (leastLaxityFirstScheduler(_pLeastLaxityFirst, observationTimeLLF, _timeLine, _executionTime) == ERROR)
	{
		printf("Error: while running leastLaxityFirstScheduler algorithm.\n");
		exit(1);
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
	_pLeastLaxityFirst = (leastLaxityFirst*)malloc(sizeof(leastLaxityFirst));

	void (*ptr)() = &getDataFromGUI;
	_ptrGetFromGUI = ptr;
	runGUI(argc, argv);

	getDataFromGUI();

	createPresentation();

	if (_edfFlag)
	{
		runEarliestDeadlineFirstScheduler();
		// runLeastLaxityFirstScheduler();
	}

	if (_rmFlag)
	{
		runRateMonothonicScheduler();
	}
	
	if (_llfFlag)
	{
		runLeastLaxityFirstScheduler();
	}

	if (!_edfFlag && !_rmFlag && !_llfFlag)
	{
		printf("No algorithm selected.\n");
	}

	// clean up section
	if (_pEarliestDeadlineFirst != NULL)
	{
		free(_pEarliestDeadlineFirst);
	}

	if (_pRateMonothonic != NULL)
	{
		free(_pRateMonothonic);
	}

	if (_pLeastLaxityFirst != NULL)
	{
		free(_pLeastLaxityFirst);
	}

	if (_executionTime != NULL)
	{
		free(_executionTime);
	}

	if (_remainTime != NULL)
	{
		free(_remainTime);
	}

	if (_timeLine != NULL)
	{
		free(_timeLine);
	}

	finistPresentation();

	return 0;
}
