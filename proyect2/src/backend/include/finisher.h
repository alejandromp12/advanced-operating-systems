#ifndef FINISHER_FILE
#define FINISHER_FILE

#include <stdio.h>

int removeBuffer(char *bufferName);
int killProducers(char *bufferName);
int killConsumers(char *bufferName);

#endif // FINISHER_FILE