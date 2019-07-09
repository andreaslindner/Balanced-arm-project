#include <rMotor.h>
#include <lMotor.h>

void Init_Motor()
{
	Init_rMotor();
	Init_lMotor();
}

void Motor_Start()
{
	rMotor_Start();
	lMotor_Start();
}

void Motor_Stop()
{
	rMotor_Stop();
	lMotor_Stop();
}

void Motor_changeDir(uint8_t direction)
{
	rMotor_changeDir(direction);
	lMotor_changeDir(direction);
}

void Motor_Backward(uint8_t perPower)
{
	rMotor_Backward(perPower);
	lMotor_Backward(perPower);
}

void Motor_Forward(uint8_t perPower)
{
	rMotor_Forward(perPower);
	lMotor_Forward(perPower);
}
