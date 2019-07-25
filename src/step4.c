#include <chip.h>
#include <i2c.h>
#include <imu.h>
#include <pinint.h>
#include <uart.h>
#include <fonction.h>
#include <motor.h>
#include <calibrate.h>

volatile uint8_t function = 1;							//Useful for communication between IMU handler and main loop
volatile int16_t values[7] = {0,0,0,0,0,0,0};			//Values of the IMU

volatile float kp = 0;
volatile float ki = 0;
volatile float kd = 0;
volatile float alpha = 0.9;

int main()
{
	uint8_t init_system = 1;
	char MESS[32] = "11111111111111111111111111111111";

	while (init_system) {
		Chip_GPIO_Init(LPC_GPIO);			//Init chip
		Init_UART();						//Init UART
		Init_Motor();						//Init both motors
		init_system = Init_IMU();			//Init IMU (and I2C in a row)
	}
	//calculate_offset(ACC_X_OFF, ACC_Z_OFF, GYRO_Y_OFF);

	Motor_Start();
	Init_PININT();
	
	UART_Send_SHA256(MESS, 32);
	while(1) {	//main loop -> reading UART

		UART_Read_Replay();
	}
	return(0);
}
