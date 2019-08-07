#include <stdint.h>

uint8_t ax_per(volatile const int16_t values[7])
{
	//absolute value
	uint16_t value = (values[0] < 0) ? -values[0] : values[0];
	//return the per value/maxValue
	return(value*100/32767);
}

uint8_t mult_per(uint8_t number, volatile const int16_t values[7])
{
	//absolute value
	uint16_t value = (values[number-1] < 0) ? -values[number-1] : values[number-1];
	//return the per value/maxValue
	return(value*100/32767);
}

uint8_t direction(volatile const int16_t values[7])
{
	return (values[5] < 0 ? 0 : 1);
}

float translate(int16_t value, int lastMinValue, int lastMaxValue, int newMinValue, int newMaxValue)
{
	int lastRange = lastMaxValue - lastMinValue;
	int newRange = newMaxValue - newMinValue;

	if (lastRange != 0) {
		return((float)((value * newRange) / ((float)lastRange)));
	} else {
		return(0);
	}
}
