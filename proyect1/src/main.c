#include <stdio.h>
#include <time.h>

#include "backend/include/threadsManager.h"
#include "frontend/include/gui.h"



int main(int argc, char *pArgv[])
{
	run(argc, pArgv, 10);
	
	sleep(3);
	
	updateUI(7, 65, 3.1415);
	
	return 0;
}

