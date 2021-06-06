#ifndef BEAMER_FILE
#define BEAMER_FILE

#include <stdio.h>
#include <stdlib.h>  
#include <string.h>
#include <wchar.h>

FILE *OUTPUT;

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
#endif // BEAMER_FILE
