#ifndef RMOTOR_H_
#define RMOTOR_H_

#define PERIOD_RESET_RMOTOR 256 // Number of tick for pwm period
#define RFREQ_LOOP 10000		// Frequency of the loop

#include <stdint.h>

#define FORWARD 10
#define BACKWARD 11

void Init_rMotor();										// Init and configure right motor, start in forward mode
void rMotor_Start();									// Start forward 0% (you have to run this function before moving)
void rMotor_Stop();										// Stop rMotor
void rMotor_changeDir(uint8_t direction);				// Change direction (backward 11 or forward 10)
uint8_t rMotor_isForward();								// Return current direction (backward 11 or forward 10)
void rMotor_Backward(uint8_t perPower);					// Change right wheel speed while going backward ( perPower : percent of the power we want)  (percent on 255)
void rMotor_Forward(uint8_t perPower);					// Change right wheel speed while going forward ( perPower : percent of the power we want)  (percent on 255)

#endif /* RMOTOR_H_ */
