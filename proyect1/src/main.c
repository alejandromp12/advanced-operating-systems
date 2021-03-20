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
    char *nextThread = (char*)malloc(2 * sizeof(char));
    sprintf(nextThread, "t%d", counter);

    if(!strcmp(nextThread, argv[index]))
    {
        for (int k = index + 1; k <= index + 3; k++)
        {
            if (!(isdigit(*argv[k]) && atoi(argv[k]) > 0))
            {
                printf("Error thread attribute ==> -t%d <num_ticket:int> <workload:int> <quantum:int>\n\n", counter);
                argumentsError();
            }
        }
        int totalTickets = atoi(argv[index + 1]);
        int workload = atoi(argv[index + 2]);
        int quantum = atoi(argv[index + 3]);
        int startTerm = TOTAL_TERM;
        //TEST TICKETS
        int tickets[7] = {9, 50, 3, 1, 4, 0, 2};


        populateWorker(&WORKER_LIST[counter], &tickets, totalTickets, startTerm, workload, quantum);
        free(nextThread);

        TOTAL_TERM += (workload * UNIT_OF_WORK);
        printf("CREATE THREAD %c!!\n", argv[index][1]);
    }
    else
    {
        printf("Error thread => %s is wrong , the next thread must be %s\n", argv[index], nextThread);
         argumentsError();
    }
            
    
    

}

void readAttriThreads(int index, int argc, char const *argv[])
{     
    if((argc - index) ==  (NUM_THREADS * 4))
    {   
        WORKER_LIST = (thread*)malloc(NUM_THREADS * sizeof(thread));

        int counter = 0;
        for (int i = index; i < argc; i++)
        {
            createThread(i, counter, argv);
            i += 3;
            counter++;
        }
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
    }

    else
    {
        argumentsError();
    }
}


/*TEST SERIES*/

void testSerie()
{

}













int main(int argc, char const *argv[])
{
    readArguments(argc, argv);

    printf("EXPROPRIATION_MODE %d\n", EXPROPRIATION_MODE);
    printf("NUM_THREADS %d\n", NUM_THREADS);
    return 0;
}




