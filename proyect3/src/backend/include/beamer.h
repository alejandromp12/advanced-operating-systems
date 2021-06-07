#ifndef BEAMER_FILE
#define BEAMER_FILE

#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <wchar.h>

FILE *OUTPUT;



typedef struct 
{	
	int time;
	int numProcess;
	int processList[100]; //depend time
	int deadline[100]; //depend numprocess
	int deadProcess;
	int t_deadProcess;
	int executionTime[100];  //depend numprocess
	int isValid;
    int isDone;

} RTSchedulerData;

RTSchedulerData RMData;
RTSchedulerData EDFData;
RTSchedulerData LLFData;


void setCover();

void setFrame(char *title, char *content);

void finistPresentation();

void startPresentation();

void InitPresentation();

void definitionRM();

void definitionEDF();

void definitionLLF();

void createPresentation();

void simulationStep(char *title, int processList[], int cycles, int numProcesses, int deadline[], int deadProcess, int t_deadProcess, int executionTime[]);

void simulationAllAlgorithm(int cycles);

void SchedulabilityTest(int executionTime[], int deadline[], int numProcesses);

#endif // BEAMER_FILE
