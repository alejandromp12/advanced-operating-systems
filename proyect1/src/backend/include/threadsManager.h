#include <stdio.h>


typedef enum
{
    EXPROPRIATED = 0,
    NON_EXPROPRIATED = 1
} OperationModeEnum; ///<

typedef struct
{
    int tickets;
    int workLoad;
    int quantum;
    OperationModeEnum mode;
} *thread; ///<

///
void populateWorker(void *pThread, int tickets, int workLoad, int quantum, OperationModeEnum mode);

///
void updateQuantum(void *pThread, int newQuantum);

///
void updateWorkLoad(void *pThread, int newWorkLoad);