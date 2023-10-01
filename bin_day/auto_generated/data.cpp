#include <stdint.h>
#include "data.hpp"

t_binDay setArray[NUMBER_OF_DATASETS]

// --- Data from landfill.csv

uint64_t landfillUnixArray[] = {
	1693954800,
	1695164400,
	1696374000,
	1697583600,
	1698796800,
	1700006400,
	1701216000,
	1702425600,
	1703808000,
	1704931200,
	1706054400,
	1707264000,
	1708473600,
	1709683200,
	1710892800,
	1712098800,
	1713308400,
	1714518000,
	1715727600,
	1716937200,
	1718146800,
	1719356400,
	1720566000,
	1721775600,
};
uint16_t landfillUnixArrayLength = 24;
uint8_t landfillUnixArrayLedPin = 10;

// --- Data from recycling.csv

uint64_t recyclingUnixArray[] = {
	1694646000,
	1695855600,
	1697065200,
	1698274800,
	1699488000,
	1700697600,
	1701907200,
	1703116800,
	1704499200,
	1705536000,
	1706745600,
	1707955200,
	1709164800,
	1710374400,
	1711584000,
	1712790000,
	1713999600,
	1715209200,
	1716418800,
	1717628400,
	1718838000,
	1720047600,
	1721257200,
};
uint16_t recyclingUnixArrayLength = 23;
uint8_t recyclingUnixArrayLedPin = 9;

// Array of structs definition

t_binDay setArray[NUMBER_OF_DATASETS] = {
	{.arrayStart = landfillUnixArray, .arrayLength = landfillUnixArrayLength, .ledPin = landfillUnixArrayLedPin},
	{.arrayStart = recyclingUnixArray, .arrayLength = recyclingUnixArrayLength, .ledPin = recyclingUnixArrayLedPin},
};
