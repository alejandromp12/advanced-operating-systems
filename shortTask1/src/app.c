#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_NUM_OF_PROGRAM_ARGS 3
#define MAX_NUM_OF_CARS_IN_BRIDGE 2
#define NO_CARS_IN_BRIDGE 0

typedef enum
{
	EAST_TO_WEST = 0,
	WEST_TO_EAST,
	MAX_DIRECTION_INDEX
} CarsDirectionEnum;

pthread_mutex_t _mutexLogger = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexBridgeMonitor = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t _mutexBridgeDirectionResource = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t _bridgeDirectionCondition = PTHREAD_COND_INITIALIZER;
int _numCarsCrossingBridge = 0;
CarsDirectionEnum _dirCarsCrossingBridge = MAX_DIRECTION_INDEX;

typedef struct
{
	CarsDirectionEnum carDirection;
	int amountOfCars;
} carsPerDirection;


double randomExponentialDistribution(double lambda)
{
	double intermediate = 0.0;
	intermediate = rand() / (RAND_MAX + 1.0);
	return -log(1 - intermediate) / lambda;
}


void releaseBridgeResource()
{
	pthread_mutex_lock(&_mutexLogger);
	printf("A car has leave the bridge...\n");
	pthread_mutex_unlock(&_mutexLogger);

	pthread_mutex_lock(&_mutexBridgeMonitor);

	if (_numCarsCrossingBridge > NO_CARS_IN_BRIDGE)
	{
		--_numCarsCrossingBridge;
	}

	if (_numCarsCrossingBridge == NO_CARS_IN_BRIDGE)
	{
		_dirCarsCrossingBridge = MAX_DIRECTION_INDEX;
		pthread_mutex_lock(&_mutexBridgeDirectionResource);
		pthread_cond_signal(&_bridgeDirectionCondition);
		pthread_mutex_unlock(&_mutexBridgeDirectionResource);
	}

	pthread_mutex_unlock(&_mutexBridgeMonitor);
}


void acquireBridgeResource(CarsDirectionEnum carDirection)
{
	pthread_mutex_lock(&_mutexBridgeMonitor);

	if (_numCarsCrossingBridge < MAX_NUM_OF_CARS_IN_BRIDGE)
	{
		++_numCarsCrossingBridge;
	}

	_dirCarsCrossingBridge = carDirection;

	pthread_mutex_unlock(&_mutexBridgeMonitor);
}


void scheduleAcquireBridgeResource(CarsDirectionEnum carDirection)
{
	pthread_mutex_lock(&_mutexBridgeDirectionResource);
	pthread_cond_wait(&_bridgeDirectionCondition, &_mutexBridgeDirectionResource);
	pthread_mutex_unlock(&_mutexBridgeDirectionResource);

	pthread_mutex_lock(&_mutexLogger);
	printf("A car that was waiting for enter to the bridge finally achieved it...\n");
	pthread_mutex_unlock(&_mutexLogger);

	pthread_mutex_lock(&_mutexBridgeMonitor);
	++_numCarsCrossingBridge;
	_dirCarsCrossingBridge = carDirection;
	pthread_mutex_unlock(&_mutexBridgeMonitor);

	// 2 seconds crossing the bridge
	sleep(2);
	releaseBridgeResource();
}


void *bridgeMonitor(void *pCarData)
{
	pthread_mutex_lock(&_mutexLogger);
	printf("A car arrived to the bridge...\n");
	pthread_mutex_unlock(&_mutexLogger);

	carsPerDirection *pCarDirection = (carsPerDirection*)pCarData;
	if (pCarDirection) // sanity check
	{
		pthread_mutex_lock(&_mutexBridgeMonitor);
		int numCarsCrossingBridge = _numCarsCrossingBridge;
		CarsDirectionEnum carsCrossingBridgeDirection = _dirCarsCrossingBridge;
		CarsDirectionEnum carAttemptingCrossBridgeDirection = pCarDirection->carDirection;
		pthread_mutex_unlock(&_mutexBridgeMonitor);

		if (numCarsCrossingBridge == NO_CARS_IN_BRIDGE)
		{
			pthread_mutex_lock(&_mutexLogger);
			printf("A car entereded to the bridge due to there were no cars inside...\n");
			pthread_mutex_unlock(&_mutexLogger);

			acquireBridgeResource(carAttemptingCrossBridgeDirection);

			// 2 seconds crossing the bridge
			sleep(2);
			releaseBridgeResource();
		}
		else if ((numCarsCrossingBridge > NO_CARS_IN_BRIDGE)
				 && (numCarsCrossingBridge < MAX_NUM_OF_CARS_IN_BRIDGE))
		{
			if (carAttemptingCrossBridgeDirection == carsCrossingBridgeDirection)
			{
				pthread_mutex_lock(&_mutexLogger);
				printf("A car entereded to the bridge, there was/were a car(s) inside, but going to the same direction...\n");
				pthread_mutex_unlock(&_mutexLogger);

				acquireBridgeResource(carAttemptingCrossBridgeDirection);

				// 2 seconds crossing the bridge
				sleep(2);
				releaseBridgeResource();
			}
			else
			{
				pthread_mutex_lock(&_mutexLogger);
				printf("A car is waiting for enter to the bridge, bridge is not full, but all cars inside are going to the opposite direction...\n");
				pthread_mutex_unlock(&_mutexLogger);

				// wait for bridge resource
				scheduleAcquireBridgeResource(carAttemptingCrossBridgeDirection);
			}
		}
		else
		{
			pthread_mutex_lock(&_mutexLogger);
			printf("A car is waiting for enter to the bridge, since it is full of cars going to the opposite direction...\n");
			pthread_mutex_unlock(&_mutexLogger);

			// wait for bridge resource
			scheduleAcquireBridgeResource(carAttemptingCrossBridgeDirection);
		}

		// kill the thread
		pthread_exit(NULL);
	}

	// kill the thread
	//pthread_exit(NULL);
}


int main(int argc, char *pArgv[])
{
	// sanity check
	if (!pArgv)
	{
		printf("Error: pArgv is NULL.\n");
		return 1;
	}

	if (argc != MAX_NUM_OF_PROGRAM_ARGS)
	{
		printf("Usage: $ %s numCarsEastToWest numCarsWestToEast\n", pArgv[0]);
		return 1;
	}

	carsPerDirection carsEastToWest;
	carsEastToWest.carDirection = EAST_TO_WEST;
	carsEastToWest.amountOfCars = atoi(pArgv[1]);

	carsPerDirection carsWestToEast;
	carsWestToEast.carDirection = WEST_TO_EAST;
	carsWestToEast.amountOfCars = atoi(pArgv[2]);

	// Intializes time for random number generator
	time_t t;
	srand((unsigned)time(&t));

	int totalCars = carsWestToEast.amountOfCars + carsEastToWest.amountOfCars;
	pthread_t cars[totalCars];

	printf("Program started...\n");

	for (int i = 0; i < totalCars; i++)
	{
		// use random pick up
		if ((carsEastToWest.amountOfCars > 0) && (carsWestToEast.amountOfCars > 0))
		{
			pthread_mutex_lock(&_mutexLogger);
			printf("This is a random car departing selection...\n");
			pthread_mutex_unlock(&_mutexLogger);

			int randomCarDirection = rand() % MAX_DIRECTION_INDEX;
			switch (randomCarDirection)
			{
				case EAST_TO_WEST:
					pthread_mutex_lock(&_mutexLogger);
					printf("Car departed from east to west...\n");
					pthread_mutex_unlock(&_mutexLogger);

					//pthread_mutex_lock(&_mutexCarsInBridge);
					--carsEastToWest.amountOfCars;
					//pthread_mutex_unlock(&_mutexCarsInBridge);

					pthread_create(cars + i, NULL, bridgeMonitor, &carsEastToWest);
					break;

				case WEST_TO_EAST:
					pthread_mutex_lock(&_mutexLogger);
					printf("Car departed from west to east...\n");
					pthread_mutex_unlock(&_mutexLogger);

					//pthread_mutex_lock(&_mutexCarsInBridge);
					--carsWestToEast.amountOfCars;
					//pthread_mutex_unlock(&_mutexCarsInBridge);

					pthread_create(cars + i, NULL, bridgeMonitor, &carsWestToEast);
					break;

				default:
					pthread_mutex_lock(&_mutexLogger);
					printf("No car departed, unknown case...\n");
					pthread_mutex_unlock(&_mutexLogger);
					break;
			}
		}
		else if (carsEastToWest.amountOfCars > 0)
		{
			pthread_mutex_lock(&_mutexLogger);
			printf("Car departed from east to west...\n");
			pthread_mutex_unlock(&_mutexLogger);

			//pthread_mutex_lock(&_mutexCarsInBridge);
			--carsEastToWest.amountOfCars;
			//pthread_mutex_unlock(&_mutexCarsInBridge);

			pthread_create(cars + i, NULL, bridgeMonitor, &carsEastToWest);
		}
		else if (carsWestToEast.amountOfCars > 0)
		{
			pthread_mutex_lock(&_mutexLogger);
			printf("Car departed from west to east...\n");
			pthread_mutex_unlock(&_mutexLogger);

			//pthread_mutex_lock(&_mutexCarsInBridge);
			--carsWestToEast.amountOfCars;
			//pthread_mutex_unlock(&_mutexCarsInBridge);

			pthread_create(cars + i, NULL, bridgeMonitor, &carsWestToEast);
		}
	}

	// finish until all cars have crossed the bridge
	for (int i = 0; i < totalCars; i++)
	{
		pthread_join(cars + i, NULL);
	}

	printf("Program finished...\n");
	return 0;
}
