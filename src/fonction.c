#include <stdint.h>

uint8_t ax_per(const uint16_t values[7])
{
	//absolute value
	uint16_t value = (values[0] >> 15) ? (~values[0]) + 1 : values[0];
	//return the per value/maxValue
	return(value*100/32767);
}

uint8_t mult_per(uint8_t number, volatile const uint16_t values[7])
{
	//absolute value
	uint16_t value = (values[number-1] >> 15) ? (~values[number-1]) + 1 : values[number-1];
	//return the per value/maxValue
	return(value*100/32767);
}
