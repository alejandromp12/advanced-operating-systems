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
#include "backend/include/realTimeSchedulingFactory.h"
#include "backend/include/rateMonothonicScheduler.h"
#include "backend/include/earliestDeadlineFirstScheduler.h"
#include "backend/include/leastLaxityFirstScheduler.h"
//#include "frontend/include/gui.h"

static int APPLICATION_ARGS_NUM = 3;

// sincronizes GUI elements
void updateGUI()
{

}

int main(int argc, char *argv[])
{
	// sanity check
	if (!argv)
	{
		printf("Error: argv is NULL.\n");
		return 1;
	}

	if (argc != APPLICATION_ARGS_NUM)
	{
		printf("Usage: $ %s ....\n", argv[0]);
		return 1;
	}


	//void (*ptr)() = &updateGUI;
	//_ptrUpdateGUI = ptr;


	//runGUI(argc, argv, bufferId);

	return 0;
}
