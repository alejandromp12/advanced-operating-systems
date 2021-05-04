#include "include/common.h"

#include <string.h>

// Concatenates a char base name with an integer
char *getBufferName(char *baseName, int id)
{
	char bufferName[50];
	strcpy(bufferName, baseName);

	char intToString[5];
	sprintf(intToString, "%d", id);

	return strcat(bufferName, intToString);
}
