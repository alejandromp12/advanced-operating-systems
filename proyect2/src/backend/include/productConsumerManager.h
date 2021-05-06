#ifndef PRODUCT_CONSUMER_MANAGER_FILE
#define PRODUCT_CONSUMER_MANAGER_FILE

#include <stdio.h>

typedef enum
{
	PRODUCER_ROLE = 0,
	CONSUMER_ROLE = 1
} productConsumerRole;


typedef enum
{
	DECREASE= 0,
	INCREASE = 1
} productConsumerAction;


int addProductConsumer(productConsumerRole role, char *bufferName);

int removeProductConsumer(productConsumerRole role, char *bufferName);

int getProductConsumers(productConsumerRole role, char *bufferName);

void logProductConsumerAction(char *bufferName, productConsumerRole role, int bufferIndex);

#endif // PRODUCT_CONSUMER_MANAGER_FILE