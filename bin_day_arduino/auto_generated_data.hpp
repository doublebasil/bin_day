#include <stdint.h>

#define NUMBER_OF_DATASETS ( 2 )

typedef struct
{
	const uint64_t *arrayStart;
	const uint16_t arrayLength;
	const uint8_t ledPin;
} t_binDay;

const extern t_binDay setArray[NUMBER_OF_DATASETS];
