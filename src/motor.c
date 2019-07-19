#include <rMotor.h>
#include <lMotor.h>
#include <stdlib.h>
#include <uart.h>
#include <math.h>

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

static void deal_with_low_values(int *motorPower, int minValue, int minRunValue)
{
	if (*motorPower >= 0) {
		if ((*motorPower >= minRunValue) && (*motorPower <= minValue)) {
			*motorPower = minValue;
		}
	} else {
		if ((*motorPower <= -minRunValue) && (*motorPower >= -minRunValue)) {
			*motorPower = -minRunValue;
		}
	}
}

void Motor_setPower(float motorPower)
{
	int motorPowerFloor = floor(motorPower);
	motorPowerFloor = (motorPowerFloor <= -255) ? -255 : ((motorPowerFloor >= 255) ? 255 : motorPowerFloor);			//constrain the range
	//deal_with_low_values(&motorPowerFloor);

	/* DEBUG */
	/*UART_PutINT(motorPowerFloor);
	UART_PutSTR("\r\n");*/

	if (motorPowerFloor <= 0){
		Motor_Forward(abs(motorPowerFloor));
	} else {
		Motor_Backward(abs(motorPowerFloor));
	}
}
