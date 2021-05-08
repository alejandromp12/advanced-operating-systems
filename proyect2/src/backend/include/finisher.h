#ifndef FINISHER_FILE
#define FINISHER_FILE

#include <stdio.h>

int removeBuffer(char *bufferName);
int killProducer(char *bufferName);
int killConsumer(char *bufferName);

#endif // FINISHER_FILE