#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "backend/include/threadsManager.h"
#include "backend/include/scheduler.h"

#define MIN_THREAD 5
#define EXPROPRIATION 0
#define NON_EXPROPRIATION 1

int NUM_THREADS = MIN_THREAD;
int EXPROPRIATION_MODE = -1;
int TOTAL_TERM = 0;
int TOTAL_TICKETS = 0;
int TICKET_LIST_SIZE;
int *TICKET_LIST;
int *pTickets;




void argumentsError()
{
    printf("Error argument. The format is:\n\n"
           "./project <-m|--mode> <expropriation_type> <-t|--threads> <num_threads:int> -t0 <num_ticket:int> <workload:int> <quantum:int> ... -tN <num_ticket:int> <workload:int> <quantum:int>\n\n"
           "Help:\n\n"
           "<-m | --mode> ==> mode\n"
           "<expropriation_type> ==> 0 for expropriation | 1 for non-expropriation\n"
           "<-t| --threads> ==> set num threads\n"
           "<num_threads> ==> num threads, N >= 5\n"
           "<-t0> ==> set num_ticket and workload for thread number 0\n"
           "<num_ticket> => num_ticket > 0\n"
           "<workload> ==> workload > 0\n"
           "quantum ==> if the program is in expropriation mode the quantum must be in milliseconds\n"
           "   |    ==> if the program is in non-expropriation mode, the quantum must be a percentage of the total work\n"
           "<-tN> ==> set num_ticket and workload for thread number N, 1 =< N =< num_threads\n\n");
    exit(0);
}

void setTicketsList()
{   
    TICKET_LIST_SIZE = TOTAL_TICKETS;
    TICKET_LIST = (int*) malloc (TOTAL_TICKETS * sizeof(int));

    for(int i = 0; i < TOTAL_TICKETS; i++)
    {
        TICKET_LIST[i] = i;
    }
}


void removeElementTicketList(int index)
{
     for(int i = index; i < TICKET_LIST_SIZE - 1; i++)
    {
        TICKET_LIST[i] = TICKET_LIST[i + 1];
    }
    TICKET_LIST_SIZE -= 1;
}

void distributeTickets(int *threadTicket, int threadTotalTicket)
{   
    int ticket = 0;

    for(int k = 0; k < threadTotalTicket; k++)  // [ 3, 4, 5]
    {
        ticket = rand() % TICKET_LIST_SIZE; 
        threadTicket[k] = TICKET_LIST[ticket];
        removeElementTicketList(ticket);
    }
}

void readMode(int index, char const *argv[])
{
    if ((!strcmp(argv[index], "--mode") || !strcmp(argv[index], "-m")) && (!strcmp(argv[index + 1], "0") || !strcmp(argv[index + 1], "1")))
    {
        EXPROPRIATION_MODE = atoi(argv[index + 1]);
    }
    else
    {
        printf("Error 'mode' ==> <-m|--mode> <0|1>\n\n");
        argumentsError();
    }
}


void readNumThreads(int index, char const *argv[])
{

    if ((!strcmp(argv[index], "--threads") || !strcmp(argv[index], "-t")) && isdigit(*argv[index + 1]))
    {

        int num_thread = atoi(argv[index + 1]);

        if (num_thread < MIN_THREAD)
        {
            printf("Error 'num threads' ==> <-t|--threads> <N>, N >= 5\n\n");
            argumentsError();
        }

        NUM_THREADS = num_thread;
    }
    else
    {
        printf("Error 'num threads' ==> <-t|--threads> <N>, N >= 5\n\n");
        argumentsError();
    }
}


void createThread(int index, int counter, char const *argv[])
{   
    int totalTickets = atoi(argv[index + 1]);
    int workload = atoi(argv[index + 2]);
    int quantum = atoi(argv[index + 3]);
    int startTerm = TOTAL_TERM;
    int *tickets = (int *) malloc(totalTickets * sizeof(int));
    distributeTickets(tickets, totalTickets);
    populateWorker(&WORKER_LIST[counter], tickets, totalTickets, startTerm, workload, quantum, counter);

    TOTAL_TERM += (workload * UNIT_OF_WORK);
    printf("CREATED THREAD %c!!\n", argv[index][1]);

        
}


void threadValidator(int index, int counter, char const *argv[])
{   
    char *nextThread = (char*)malloc(2 * sizeof(char));
    sprintf(nextThread, "t%d", counter);

    if(strcmp(nextThread, argv[index]))
    {
        printf("Error thread => %s is wrong , the next thread must be %s\n", argv[index], nextThread);
        argumentsError();
    }
    for (int k = index + 1; k <= index + 3; k++)
    {   

        if (!(isdigit(*argv[k]) && atoi(argv[k]) > 0))
        {
            printf("Error thread attribute ==> -t%d <num_ticket:int> <workload:int> <quantum:int>\n\n", counter);
            argumentsError();
        }
        else if((k - index) == 1)
        {
            TOTAL_TICKETS += atoi(argv[k]);
        }
    }
            
}


void readAttriThreads(int index, int argc, char const *argv[])
{     
    if((argc - index) ==  (NUM_THREADS * 4))
    {   
        WORKER_LIST = (thread*)malloc(NUM_THREADS * sizeof(thread));

        int counter = 0;
        TOTAL_TICKETS = 0; //global variable
        for (int i = index; i < argc; i++)
        {
            threadValidator(i, counter, argv);
            i += 3;
            counter++;
        }

        setTicketsList();
        counter = 0;
        
        for (int k = index; k < argc; k++)
        {
            createThread(k, counter, argv);
            k += 3;
            counter++;
        }

        pTickets = (int *) malloc (TOTAL_TICKETS * sizeof(int));

    }
    else
    {
        printf("Error !!\n"); //Replace this error
        argumentsError();
    }
}


void readArguments(int argc, char const *argv[])
{
    int index = 1;
    if (argc > 1)
    {
        readMode(index, argv);
        index += 2;

        readNumThreads(index, argv);
        index += 2;

        readAttriThreads(index, argc, argv);

        TOTAL_PI = 0.0;
    }

    else
    {
        argumentsError();
    }

}
/*TEST*/     
/*#define TOTAL_TICKETS 100

int main1(int argc, char *pArgv[])
{
	int numWorkers = 5;
	int tickets1[7] = {9, 50, 3, 1, 4, 0, 2};
	int tickets2[2] = {8, 51};
	int tickets3[6] = {10, 45, 15, 19, 31, 21};
	int tickets4[3] = {85, 99, 80};
	int tickets5[1] = {100};

	thread *pWorker = (thread*)malloc(numWorkers * sizeof(thread));
	if (pWorker == NULL)
	{
		printf("Error, main(...) detected a Null pointer.\n");
	}

	for (int i = 0; i < numWorkers; i++)
	{
		if (i == 0)
		{
			populateWorker(&pWorker[i], tickets1, 7, 3, 1, i);
		}
		else if (i == 1)
		{
			populateWorker(&pWorker[i], tickets2, 2, 3, 1, i);
		}
		else if (i == 2)
		{
			populateWorker(&pWorker[i], tickets3, 6, 3, 1, i);
		}
		else if (i == 3)
		{
			populateWorker(&pWorker[i], tickets4, 3, 3, 1, i);
		}
		else if (i == 4)
		{
			populateWorker(&pWorker[i], tickets5, 1, 3, 1, i);
		}
	}

	scheduler systemScheduler;
	int *pTickets = (int*)malloc(TOTAL_TICKETS * sizeof(int));
	initializeScheduler(&systemScheduler, EXPROPRIATED_MODE, pTickets, TOTAL_TICKETS);
	validateTickets(tickets1, 7, pTickets);
	validateTickets(tickets2, 2, pTickets);
	validateTickets(tickets3, 6, pTickets);
	validateTickets(tickets4, 3, pTickets);
	validateTickets(tickets5, 1, pTickets);

	while (haveValidTickets(pTickets) > 0)
	{
		lotteryChooseNextWorker(&systemScheduler, pWorker, numWorkers, pTickets);
		// simulate that the worker has to be removed
		thread *pThread = (thread*)systemScheduler.pPrevWorker;
		if (pThread != NULL)
		{
			// remove its tickest
			invalidateTickets(pThread->pTickets, pThread->totalTickets, pTickets);
		}
	}

	free(pWorker);
	free(pTickets);
	return 0;
}


*/

int main(int argc, char const *argv[])
{   
    time_t t;
	srand((unsigned)time(&t));

    readArguments(argc, argv);

    /* for(int i = 0; i < NUM_THREADS; i++)
    {
        piCalculate(&WORKER_LIST[i]);
    }
    */

    printf("EXPROPRIATION_MODE %d\n", EXPROPRIATION_MODE);
    printf("NUM_THREADS %d\n total PI %f\n TOTAL_tickets %d\n TICKET SIZE %d\n", NUM_THREADS, TOTAL_PI, TOTAL_TICKETS, TICKET_LIST_SIZE);
    return 0;
}




