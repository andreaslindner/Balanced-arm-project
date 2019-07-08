#ifndef RMOTOR_H_
#define RMOTOR_H_

#define PERIOD_RESET_RMOTOR 100 // Number of tick ( = 0.01 ms ) for pwm period
#include <stdint.h>

#define FORWARD 10
#define BACKWARD 11

void Init_rMotor();										// Init and configure right motor, start in forward mode
void rMotor_Start();									// Start forward 0% (you have to run this function before moving)
void rMotor_Stop();										// Stop rMotor
void rMotor_changeDir(uint8_t forward);					// Change direction (backward 11 or forward 10)
uint8_t rMotor_isForward();								// Return current direction (backward 0 or forward 1)
void rMotor_Backward(uint8_t perPower);					// Change right wheel speed while going backward ( perPower : percent of the power we want)
void rMotor_Forward(uint8_t perPower);					// Change right wheel speed while going forward ( perPower : percent of the power we want)

#endif /* RMOTOR_H_ */