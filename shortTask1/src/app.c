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
	CarsDirectionEnum direction;
	int threadId;
} carsPerDirection;


void carsFactory(CarsDirectionEnum carDirection, int threadId, carsPerDirection *pCar)
{
	if (pCar)
	{
		pCar->direction = carDirection;
		pCar->threadId = threadId;
	}
}


CarsDirectionEnum getCarDirection(int totalCarsEastToWest, int totalCarsWestToEast)
{
	// use random pick up
	if ((totalCarsEastToWest > 0) && (totalCarsWestToEast > 0))
	{
		return rand() % MAX_DIRECTION_INDEX;
	}
	else if (totalCarsEastToWest > 0)
	{
		return WEST_TO_EAST;
	}
	else if (totalCarsWestToEast > 0)
	{
		return EAST_TO_WEST;
	}
	else
	{
		return MAX_DIRECTION_INDEX;
	}
}


char *getCarDirectionStr(CarsDirectionEnum carDirection)
{
	switch (carDirection)
	{
		case EAST_TO_WEST:
			return "west to east";
		case WEST_TO_EAST:
			return "east to west";
		default:
			return "unknown direction";
	}
}

double randomExponentialDistribution(double lambda)
{
	double intermediate = 0.0;
	intermediate = rand() / (RAND_MAX + 1.0);
	return -log(1 - intermediate) / lambda;
}


void releaseBridgeResource(CarsDirectionEnum carDirection, int carId)
{
	pthread_mutex_lock(&_mutexLogger);
	printf("Car %i, which is going from %s has leave the bridge...\n",
		   carId, getCarDirectionStr(carDirection));
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


void scheduleAcquireBridgeResource(CarsDirectionEnum carDirection, int carId)
{
	pthread_mutex_lock(&_mutexBridgeDirectionResource);
	pthread_cond_wait(&_bridgeDirectionCondition, &_mutexBridgeDirectionResource);
	pthread_mutex_unlock(&_mutexBridgeDirectionResource);

	pthread_mutex_lock(&_mutexLogger);
	printf("Car %i, which is going from %s and was waiting for enter to the bridge finally achieved it...\n",
		   carId, getCarDirectionStr(carDirection));
	pthread_mutex_unlock(&_mutexLogger);

	pthread_mutex_lock(&_mutexBridgeMonitor);
	++_numCarsCrossingBridge;
	_dirCarsCrossingBridge = carDirection;
	pthread_mutex_unlock(&_mutexBridgeMonitor);

	// 2 seconds crossing the bridge
	sleep(2);
	releaseBridgeResource(carDirection, carId);
}


void *bridgeMonitor(void *pCarData)
{
	carsPerDirection *pCarDirection = (carsPerDirection*)pCarData;
	if (pCarDirection) // sanity check
	{
		pthread_mutex_lock(&_mutexBridgeMonitor);
		int numCarsCrossingBridge = _numCarsCrossingBridge;
		CarsDirectionEnum carsCrossingBridgeDirection = _dirCarsCrossingBridge;
		CarsDirectionEnum carAttemptingCrossBridgeDirection = pCarDirection->direction;
		int carId = pCarDirection->threadId;
		pthread_mutex_unlock(&_mutexBridgeMonitor);

		pthread_mutex_lock(&_mutexLogger);
		printf("Car %i, which is going from %s arrived to the bridge...\n",
			   carId, getCarDirectionStr(carAttemptingCrossBridgeDirection));
		pthread_mutex_unlock(&_mutexLogger);

		if (numCarsCrossingBridge == NO_CARS_IN_BRIDGE)
		{
			pthread_mutex_lock(&_mutexLogger);
			printf("Car %i, which is going from %s entereded to the bridge, since there were no cars inside...\n",
				   carId, getCarDirectionStr(carAttemptingCrossBridgeDirection));
			pthread_mutex_unlock(&_mutexLogger);

			acquireBridgeResource(carAttemptingCrossBridgeDirection);

			// 2 seconds crossing the bridge
			sleep(2);
			releaseBridgeResource(carAttemptingCrossBridgeDirection, carId);
		}
		else if ((numCarsCrossingBridge > NO_CARS_IN_BRIDGE)
				 && (numCarsCrossingBridge < MAX_NUM_OF_CARS_IN_BRIDGE))
		{
			if (carAttemptingCrossBridgeDirection == carsCrossingBridgeDirection)
			{
				pthread_mutex_lock(&_mutexLogger);
				printf("Car %i, which is going from %s entereded to the bridge, since cars inside were going to the same direction...\n",
					   carId, getCarDirectionStr(carAttemptingCrossBridgeDirection));
				pthread_mutex_unlock(&_mutexLogger);

				acquireBridgeResource(carAttemptingCrossBridgeDirection);

				// 2 seconds crossing the bridge
				sleep(2);
				releaseBridgeResource(carAttemptingCrossBridgeDirection, carId);
			}
			else
			{
				pthread_mutex_lock(&_mutexLogger);
				printf("Car %i, which is going from %s is waiting for enter to the bridge, bridge is not full, but all cars inside are going to the opposite direction...\n",
					   carId, getCarDirectionStr(carAttemptingCrossBridgeDirection));
				pthread_mutex_unlock(&_mutexLogger);

				// wait for bridge resource
				scheduleAcquireBridgeResource(carAttemptingCrossBridgeDirection, carId);
			}
		}
		else
		{
			pthread_mutex_lock(&_mutexLogger);
			printf("Car %i, which is going from %s is waiting for enter to the bridge, since it is full of cars going to the opposite direction...\n",
				   carId, getCarDirectionStr(carAttemptingCrossBridgeDirection));
			pthread_mutex_unlock(&_mutexLogger);

			// wait for bridge resource
			scheduleAcquireBridgeResource(carAttemptingCrossBridgeDirection, carId);
		}

		// kill the thread
		pthread_exit(NULL);
	}
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

	// Intializes time for random number generator
	time_t t;
	srand((unsigned)time(&t));

	int totalCarsEastToWest = atoi(pArgv[1]);
	int totalCarsWestToEast = atoi(pArgv[2]);
	int totalCars = totalCarsWestToEast + totalCarsEastToWest;
	pthread_t threadCars[totalCars];
	carsPerDirection cars[totalCars];

	printf("Program started...\n");

	for (int i = 0; i < totalCars; i++)
	{
		CarsDirectionEnum carDirection = getCarDirection(totalCarsEastToWest, totalCarsWestToEast);
		switch (carDirection)
		{
			case EAST_TO_WEST:
				pthread_mutex_lock(&_mutexLogger);
				printf("Car %i departed from east to west...\n", i);
				pthread_mutex_unlock(&_mutexLogger);

				--totalCarsEastToWest;
				carsFactory(EAST_TO_WEST, i, &cars[i]);
				pthread_create(threadCars + i, NULL, bridgeMonitor, (void*)&cars[i]);
				break;

			case WEST_TO_EAST:
				pthread_mutex_lock(&_mutexLogger);
				printf("Car %i departed from west to east...\n", i);
				pthread_mutex_unlock(&_mutexLogger);

				--totalCarsWestToEast;
				carsFactory(WEST_TO_EAST, i, &cars[i]);
				pthread_create(threadCars + i, NULL, bridgeMonitor, (void*)&cars[i]);
				break;

			default:
				pthread_mutex_lock(&_mutexLogger);
				printf("No car departed, unknown case...\n");
				pthread_mutex_unlock(&_mutexLogger);
				break;
		}

		// finish until all cars have crossed the bridge
		pthread_join(threadCars[i], NULL);
	}

	printf("Program finished...\n");
	return 0;
}
