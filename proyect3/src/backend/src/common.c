#include "include/common.h"


//get maximum of three numbers
int getMax(int a,int b, int c, int d, int e, int f)
{
	if (a>=b && a>=c && a>=d && a>=e && a>=f)
		return (long)a;
	else if (b>=a && b>=c && b>=d && b>=e && b>=f)
		return (long)b;
	else if (c>=a && c>=b && c>=d && c>=e && c>=f)
		return (long)c;
	else if (d>=a && d>=b && d>=c && d>=e && d>=f)
		return (long)d;
	else if (e>=a && e>=b && e>=c && e>=d && e>=f)
		return (long)c;
	else if (f>=a && f>=b && f>=c && f>=d && f>=e)
		return (long)f;
	else
		return ERROR;
}


//calculating the observation time for scheduling timeline
int getObservationTime(int period[])
{
	return getMax(period[0], period[1], period[2], period[3], period[4], period[5]);
	//return max(deadline[0], deadline[1], deadline[2])
}


//print scheduling sequence
void printSchedule(int processList[], int cycles, int numProcesses)
{
	printf("Scheduling:-\n");
	printf("Time: ");
	for (int i = 0; i < cycles; i++)
	{
		if (i < 10)
		{
			printf("| 0%i ", i);
		}
		else
		{
			printf("| %d ", i);
		}
	}
	printf("|\n");

	for (int i=0; i<numProcesses; i++)
	{
		printf("P[%d]: ", i + 1);
		for (int j=0; j<cycles; j++)
		{
			if (processList[j] == i+1)
			{
				printf("|####");
			}
			else
			{
				printf("|    ");
			}
		}
		printf("|\n");
	}
}