#ifndef MOTOR_H_
#define MOTOR_H_

void Init_Motor();										// Init and configure both motors, start in forward mode
void Motor_Start();										// Start forward 0% (you have to run this function before moving)
void Motor_Stop();										// Stop both motors
void Motor_changeDir(uint8_t forward);					// Change direction (backward 11 or forward 10)
void Motor_Backward(uint8_t perPower);					// Change the speed of the wheels while going backward ( perPower : percent of the power we want)
void Motor_Forward(uint8_t perPower);					// Change the speed of the wheels while going forward ( perPower : percent of the power we want)

#endif /* MOTOR_H_ */
