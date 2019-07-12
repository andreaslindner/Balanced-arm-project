#include <chip.h>
#include <i2c.h>
#include <imu.h>
#include <pinint.h>
#include <uart.h>
#include <fonction.h>
#include <motor.h>
#include <calibrate.h>

volatile uint8_t function = 1;	//Useful for communication between IMU handler and main loop
volatile int16_t values[7] = {0,0,0,0,0,0,0};


int main()
{
	uint8_t tmp;
	uint8_t init_system = 1;

	while (init_system) {
		Chip_GPIO_Init(LPC_GPIO);			//Init chip
		Init_UART();						//Init UART
		Init_Motor();						//Init both motors
		init_system = Init_IMU();			//Init IMU (and I2C in a row)
	}
	calculate_offset(ACC_X_OFF, ACC_Z_OFF, GYRO_Y_OFF);
	Motor_Start();
	Init_PININT();


	while(1) {	//main loop -> reading UART

		tmp = UART_Read_max_nB(1);
		if (tmp != 10) {
			function = tmp;
			UART_PutSTR("Function set as : ");
			UART_PutHEX(function);
			UART_PutSTR("\r\n");
		}
	}
	return(0);
}
