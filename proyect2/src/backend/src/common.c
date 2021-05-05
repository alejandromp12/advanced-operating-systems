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

/**
 * \brief Returns the random exponential distribution of a lambda value
 *
 * \param lambda, value between 0 and 1
 *
 * \return random exponential distribution of the lambda value passed
 */
double randomExponentialDistribution(double lambda)
{
	double intermediate = 0.0;
	intermediate = rand() / (RAND_MAX + 1.0);
	return -log(1 - intermediate) / lambda;
}