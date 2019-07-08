#include <chip.h>
#include <i2c.h>
#include <imu.h>
#include <pinint.h>
#include <uart.h>
#include <fonction.h>
#include <rMotor.h>
#include <lMotor.h>

volatile uint8_t function = 1;	//Useful for communication between IMU handler and main
extern const uint8_t imu_address;
volatile int16_t values[7] = {0,0,0,0,0,0,0};

int main()
{
	/*uint8_t tmp;*/
	uint8_t init_system = 1;

	while (init_system) {
		Chip_GPIO_Init(LPC_GPIO);			//Init chip
		Init_UART();						//Init UART
		Init_lMotor();
		Init_rMotor();
		init_system = Init_IMU();			//Init IMU (and I2C in a row)
	}
	Init_PININT();
	lMotor_Start();
	rMotor_Start();
	lMotor_Forward(1);
	rMotor_Forward(90);


	while(1) {	//main loop -> reading UART

		/*tmp = UART_Read_max_nB(1);
		if (tmp != 10) {
			function = tmp;
			UART_PutSTR("Function set as : ");
			UART_PutHEX(function);
			UART_PutSTR("\r\n");
		}*/
	}
	return(0);
}
