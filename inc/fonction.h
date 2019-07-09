#ifndef FONCTION_H_
#define FONCTION_H_

uint8_t ax_per(volatile const int16_t values[7]);							//get ax value (per)
uint8_t mult_per(uint8_t number, volatile const int16_t values[7]);			//get the value (referenced by "number") (per)
uint8_t direction(volatile const int16_t values[7]);						//return 1 if values[5] >= 0, 0 otherwise. values[5] is x-gyro

#endif /* FONCTION_H_ */
