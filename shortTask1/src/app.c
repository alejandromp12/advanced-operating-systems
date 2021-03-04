#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_NUM_OF_PROGRAM_ARGS 3 ///< Indicates the max number of args in command line accepted
#define MAX_NUM_OF_CARS_IN_BRIDGE 3 ///< Indicates the max number of cars that can be on the bridge at the same time
#define NO_CARS_IN_BRIDGE 0 //< Indicates when there are no cars crossing the bridge

typedef enum
{
	EAST_TO_WEST = 0,
	WEST_TO_EAST,
	MAX_DIRECTION_INDEX
} CarsDirectionEnum; ///< Enum to handle the cars direction


pthread_mutex_t _mutexLogger = PTHREAD_MUTEX_INITIALIZER; ///< Makes logging thread-safe
pthread_mutex_t _mutexBridgeMonitor = PTHREAD_MUTEX_INITIALIZER; ///< Makes the bridge resource thread-safe
pthread_mutex_t _mutexBridgeDirectionResource = PTHREAD_MUTEX_INITIALIZER; ///< Helps to handle the bridge crossing in a thread-safe
pthread_cond_t _bridgeDirectionCondition = PTHREAD_COND_INITIALIZER; ///< Helps to wake up threads, i.e. cars that are waiting for enter to the bridge

double _eastToWestTimeConst = 0.6; ///< lambda value of the random exponential distribution for cars going from east to west
double _westToEastTimeConst = 0.8; ///< lambda value of the random exponential distribution for cars going from west to east
int _numCarsCrossingBridge = 0; ///< Indicates the current number of cars crossing the bridge
CarsDirectionEnum _dirCarsCrossingBridge = MAX_DIRECTION_INDEX; ///< Indicates the direction of the cars that are crossing the bridge


typedef struct
{
	CarsDirectionEnum direction;
	int threadId;
} carsPerDirection; ///< Struct to handle cars in the system


/**
 * \brief Populates car structures
 *
 * \param carDirection, direction of the car
 * \param threadId, thread id associated to the car
 * \param pCar, car structure
 */
void carsFactory(CarsDirectionEnum carDirection, int threadId, carsPerDirection *pCar)
{
	if (pCar)
	{
		pCar->direction = carDirection;
		pCar->threadId = threadId;
	}
}


/**
 * \brief Returns the car direction regarding some internal aspects
 *
 * \param totalCarsEastToWest, total amount of cars going from east to west
 * \param totalCarsWestToEast, total amount of cars going from west to east
 *
 * \return direction of the next car which is going to depart
 */
CarsDirectionEnum getCarDirection(int totalCarsEastToWest, int totalCarsWestToEast)
{
	// use random pick up
	if ((totalCarsEastToWest > 0) && (totalCarsWestToEast > 0))
	{
		return rand() % MAX_DIRECTION_INDEX;
	}
	else if (totalCarsEastToWest > 0)
	{
		return EAST_TO_WEST;
	}
	else if (totalCarsWestToEast > 0)
	{
		return WEST_TO_EAST;
	}
	else
	{
		return MAX_DIRECTION_INDEX;
	}
}


/**
 * \brief Returns the car direction as a text
 *
 * \param carDirection, direction of the car
 *
 * \return direction of the car as a string
 */
char *getCarDirectionStr(CarsDirectionEnum carDirection)
{
	switch (carDirection)
	{
		case EAST_TO_WEST:
			return "east to west";
		case WEST_TO_EAST:
			return "west to east";
		default:
			return "unknown direction";
	}
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


/**
 * \brief Releases a resource when a car leaves the bridge
 *
 * \param carDirection, direction of the car
 * \param carId, thread id of the car
 */
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

	pthread_mutex_lock(&_mutexLogger);
	printf("The current number of cars crossing the bridge is: %i ...\n", _numCarsCrossingBridge);
	pthread_mutex_unlock(&_mutexLogger);

	if (_numCarsCrossingBridge == NO_CARS_IN_BRIDGE)
	{
		_dirCarsCrossingBridge = MAX_DIRECTION_INDEX;
		pthread_mutex_lock(&_mutexBridgeDirectionResource);
		pthread_cond_signal(&_bridgeDirectionCondition);
		pthread_mutex_unlock(&_mutexBridgeDirectionResource);
	}

	pthread_mutex_unlock(&_mutexBridgeMonitor);
}



/**
 * \brief Acquires a resource for a car coming to the bridge
 *
 * \param carDirection, direction of the car
 */
void acquireBridgeResource(CarsDirectionEnum carDirection)
{
	pthread_mutex_lock(&_mutexBridgeMonitor);

	if (_numCarsCrossingBridge < MAX_NUM_OF_CARS_IN_BRIDGE)
	{
		++_numCarsCrossingBridge;
	}

	pthread_mutex_lock(&_mutexLogger);
	printf("The current number of cars crossing the bridge is: %i ...\n", _numCarsCrossingBridge);
	pthread_mutex_unlock(&_mutexLogger);

	_dirCarsCrossingBridge = carDirection;

	pthread_mutex_unlock(&_mutexBridgeMonitor);

	// 3 seconds crossing the bridge
	sleep(3);
}


/**
 * \brief Schedules an acquiring of a resource for a car coming to the bridge
 *
 * \param carDirection, direction of the car
 * \param carId, thread id of the car
 */
void scheduleAcquireBridgeResource(CarsDirectionEnum carDirection, int carId)
{
	pthread_mutex_lock(&_mutexBridgeDirectionResource);
	pthread_cond_wait(&_bridgeDirectionCondition, &_mutexBridgeDirectionResource);
	pthread_mutex_unlock(&_mutexBridgeDirectionResource);

	pthread_mutex_lock(&_mutexLogger);
	printf("Car %i, which is going from %s and was waiting for enter to the bridge finally achieved it...\n",
		   carId, getCarDirectionStr(carDirection));
	pthread_mutex_unlock(&_mutexLogger);

	acquireBridgeResource(carDirection);
	releaseBridgeResource(carDirection, carId);
}


/**
 * \brief Handles which thread/car can cross the bridge
 *
 * \param pCarData, pointer of the car structure which is arriving to the bridge
 */
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
			releaseBridgeResource(carAttemptingCrossBridgeDirection, carId);
		}
		else
		{
			if (numCarsCrossingBridge < MAX_NUM_OF_CARS_IN_BRIDGE)
			{
				if (carAttemptingCrossBridgeDirection == carsCrossingBridgeDirection)
				{
					pthread_mutex_lock(&_mutexLogger);
					printf("Car %i, which is going from %s entereded to the bridge, since cars inside were going to the same direction...\n",
                           carId, getCarDirectionStr(carAttemptingCrossBridgeDirection));
					pthread_mutex_unlock(&_mutexLogger);

					acquireBridgeResource(carAttemptingCrossBridgeDirection);
					releaseBridgeResource(carAttemptingCrossBridgeDirection, carId);
				}
				else
				{
					pthread_mutex_lock(&_mutexLogger);
					printf("Car %i, which is going from %s is waiting for enter to the bridge, since cars inside are going to the opposite direction...\n",
                           carId, getCarDirectionStr(carAttemptingCrossBridgeDirection));
					pthread_mutex_unlock(&_mutexLogger);

					// wait for bridge resource
					scheduleAcquireBridgeResource(carAttemptingCrossBridgeDirection, carId);
				}
			}
			else
			{
				if (carAttemptingCrossBridgeDirection == carsCrossingBridgeDirection)
				{
					pthread_mutex_lock(&_mutexLogger);
					printf("Car %i, which is going from %s is waiting for enter to the bridge, since it is full of cars going to the same direction...\n",
                           carId, getCarDirectionStr(carAttemptingCrossBridgeDirection));
					pthread_mutex_unlock(&_mutexLogger);

					// wait for bridge resource
					scheduleAcquireBridgeResource(carAttemptingCrossBridgeDirection, carId);
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
			}
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

	// general variables
	int totalCarsEastToWest = atoi(pArgv[1]);
	int totalCarsWestToEast = atoi(pArgv[2]);
	int totalCars = totalCarsWestToEast + totalCarsEastToWest;
	pthread_t threadCars[totalCars];
	carsPerDirection cars[totalCars];

	printf("Program started...\n");

	for (int i = 0; i < totalCars; i++)
	{
		int time = 0;
		CarsDirectionEnum carDirection = getCarDirection(totalCarsEastToWest, totalCarsWestToEast);
		switch (carDirection)
		{
			case EAST_TO_WEST:
				time = ceil(randomExponentialDistribution(_eastToWestTimeConst));
				pthread_mutex_lock(&_mutexLogger);
				printf("Car %i departed from east to west... time waiting: %is\n", i, time);
				pthread_mutex_unlock(&_mutexLogger);

				--totalCarsEastToWest;
				carsFactory(EAST_TO_WEST, i, &cars[i]);

				sleep(time);
				pthread_create(threadCars + i, NULL, bridgeMonitor, (void*)&cars[i]);
				break;

			case WEST_TO_EAST:
				time = ceil(randomExponentialDistribution(_westToEastTimeConst));
				pthread_mutex_lock(&_mutexLogger);
				printf("Car %i departed from west to east... time waiting: %is\n", i, time);
				pthread_mutex_unlock(&_mutexLogger);

				--totalCarsWestToEast;
				carsFactory(WEST_TO_EAST, i, &cars[i]);

				sleep(time);
				pthread_create(threadCars + i, NULL, bridgeMonitor, (void*)&cars[i]);
				break;

			default:
				pthread_mutex_lock(&_mutexLogger);
				printf("No car departed, unknown case...\n");
				pthread_mutex_unlock(&_mutexLogger);
				break;
		}
	}

	// must be out of the other for
	for (int i = 0; i < totalCars; i++)
	{
		// finish until all cars have crossed the bridge
		pthread_join(threadCars[i], NULL);
	}

	printf("Program finished...\n");
	return 0;
}
