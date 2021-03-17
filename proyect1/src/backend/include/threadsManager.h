#include <setjmp.h>
#include <stdio.h>

typedef enum
{
    EXPROPRIATED = 0,
    NON_EXPROPRIATED = 1
} OperationModeEnum; ///<

typedef enum
{
    NO_ERROR = 0,
    ERROR = 1
} ResultEnum; ///<

typedef struct
{
    int tickets;
    int workLoad; // total
    int workLoadProgress; // current
    int quantum;
    int piApprox; // PI value
    sigjmp_buf sigjmpBuf;
    OperationModeEnum mode;
} *thread; ///<

///
void populateWorker(void *pThread, int tickets, int workLoad, int quantum, OperationModeEnum mode);

///
ResultEnum updateQuantum(void *pThread, int newQuantum);

///
int updateWorkLoad(void *pThread, int newWorkLoad);
