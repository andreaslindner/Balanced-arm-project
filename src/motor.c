#include <rMotor.h>
#include <lMotor.h>
#include <stdlib.h>
#include <uart.h>
#include <math.h>

#define MOTOR_MAX 1600

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

void Motor_Backward(uint16_t perPower)
{
	rMotor_Backward(perPower);
	lMotor_Backward(perPower);
}

void Motor_Forward(uint16_t perPower)
{
	rMotor_Forward(perPower);
	lMotor_Forward(perPower);
}

/* Deal with low value issues, for example if the wheels begin to turn after 100 = 5 V * (100/255), you might want them to turn after 50, to do that set minValue to 100 and minRunValue = 50 */
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
	motorPowerFloor = (motorPowerFloor <= -MOTOR_MAX) ? -MOTOR_MAX : ((motorPowerFloor >= MOTOR_MAX) ? MOTOR_MAX : motorPowerFloor);			//constrain the range
	//deal_with_low_values(&motorPowerFloor);

	/*
	UART_PutSTR("Motor : ");
	UART_PutFLOAT((float)(motorPowerFloor*100/MOTOR_MAX),2);
	UART_PutSTR(" %\n");
	*/
	if (motorPowerFloor <= 0){
		Motor_Forward(abs(motorPowerFloor));
	} else {
		Motor_Backward(abs(motorPowerFloor));
	}
}
