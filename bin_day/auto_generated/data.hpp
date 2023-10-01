#include <stdint.h>

#define NUMBER_OF_DATASETS ( 2 )

typedef struct
{
	uint64_t *arrayStart;
	uint16_t arrayLength;
	uint8_t ledPin;
} t_binDay;

extern t_binDay setArray[NUMBER_OF_DATASETS];
