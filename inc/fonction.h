#ifndef FONCTION_H_
#define FONCTION_H_

uint8_t ax_per(const int16_t values[7]);							//get ax value (per)
uint8_t mult_per(uint8_t number, volatile const int16_t values[7]);			//get the value (referenced by "number") (per)

#endif /* FONCTION_H_ */
