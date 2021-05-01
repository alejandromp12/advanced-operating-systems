#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "backend/include/consumer.h"
#include "backend/include/factory.h"
#include "backend/include/finisher.h"
#include "backend/include/producer.h"
#if defined(CREATOR_APP)
#include "frontend/include/gui.h"
#endif

#if defined(CREATOR_APP)

int main(int argc, char  *argv[])
{
	printf("CREATOR_APP.\n");

	// simple test, just for review functionality
	int bufferSize = 3;
	if (!createSharedBuffer(bufferSize, "/home/alejandro/shared-buffer0"))
	{
		printf("Error, creating shared buffer.\n");
		return 1;
	}

	while (1);

    // ends

	return 0;
}

#elif defined(PRODUCER_APP)

int main(int argc, char  *argv[])
{
	// simple test, just for review functionality
	printf("PRODUCER_APP.\n");
	dataMessage data;
	data.producerId = 80;
	data.key = 3;

	struct tm tm;
	strftime(data.date, sizeof(data.date), "%d %b %Y %H:%M", &tm);
	tryWrite(data, "/tmp/sharedBuffer0");
    // ends

	return 0;
}

#elif defined(CONSUMER_APP)

int main(int argc, char  *argv[])
{
	// simple test, just for review functionality
	printf("CONSUMER_APP.\n");
    // ends

	return 0;
}

#elif defined(TERMINATOR_APP)

int main(int argc, char  *argv[])
{
	// simple test, just for review functionality
	printf("TERMINATOR_APP.\n");

	if (!removeBuffer("/tmp/sharedBuffer0"))
	{
		printf("Error, removing buffer.\n");
		return 1;
	}
    // ends

	return 0;
}

#endif

