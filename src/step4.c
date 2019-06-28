#include <chip.h>
#include <i2c.h>
#include <imu.h>
#include <pinint.h>
#include <uart.h>
#include <led.h>
#include <fonction.h>

volatile uint8_t function = 1;	//Useful for communication between IMU handler and main
extern const uint8_t imu_address;
volatile uint16_t values[7] = {0,0,0,0,0,0,0};

int main()
{
	uint8_t tmp;

	Chip_GPIO_Init(LPC_GPIO);			//Init chip
	Init_UART();						//Init UART
	Init_LED();							//Init LED config
	Init_TIMER_LED(PERIOD_RESET_LED);	//Init PWM for green LED and blue LED
	if (Init_IMU()) {					//Init IMU (and I2C in a row)
		UART_PutSTR("imu init failed\r\n");
	} else {
		UART_PutSTR("imu init succeeded\r\n");
	}
	Init_PININT();
	Chip_GPIO_SetPinState(LPC_GPIO,1,11,true);


	Chip_TIMER_Enable(LPC_TIMER16_0);	//Enable LED TIMER

	Set_TIMER_Match(1,10,PERIOD_RESET_LED); //Set blue LED to 10% luminosity (better to see contrast in green luminosity)

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
