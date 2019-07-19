#ifndef LMOTOR_H_
#define LMOTOR_H_

#define PERIOD_RESET_LMOTOR 256 // Number of TIMER ticks for pwm period
#define LFREQ_LOOP 1000			// Frequency of the loop

#include <stdint.h>

#define FORWARD 10
#define BACKWARD 11

void Init_lMotor();										// Init and configure left motor, start in forward mode
void lMotor_Start();									// Start forward 0% (you have to run this function before moving)
void lMotor_Stop();										// Stop lMotor
void lMotor_changeDir(uint8_t direction);				// Change direction (backward 11 or forward 12)
uint8_t lMotor_isForward();								// Return current direction (backward 11 or forward 10)
void lMotor_Backward(uint8_t perPower);					// Change left wheel speed while going backward ( perPower : percent of the power we want)  (percent on 255)
void lMotor_Forward(uint8_t perPower);					// Change left wheel speed while going forward ( perPower : percent of the power we want)  (percent on 255)

#endif /* LMOTOR_H_ */
