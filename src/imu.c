#include <LPC11xx.h>
#include <uart.h>
#include <i2c.h>

const uint8_t imu_address = 0x68;
extern volatile int16_t values[7];

uint8_t Init_IMU()
{
	uint8_t counter_timeout = 0;
	//Init AD0, pull down valuesistor
	LPC_IOCON->PIO1_10  &= ~0x1F;
	LPC_IOCON->PIO1_10  |= 0x08;

	//Init i2c bus
	I2CInit(I2CMASTER);

	while(I2C_Write_Blocking_1B(imu_address, 0x19, 0x04) == 2) {	//Set the sample rate to 100 Hz when DLPF is enabled, otherwise it's set to 800 Hz.
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	while(I2C_Write_Blocking_1B(imu_address, 0x1B, 0x08) == 2) {	//Gyro scale range set to +- 250°/s, LSB sensitivity 131 LSB/(°/s)
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	while(I2C_Write_Blocking_1B(imu_address, 0x1C, 0x10) == 2) {	//Acc scale range set to +- 2g, LSB sensitivity 16 384 LSB/g
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	while(I2C_Write_Blocking_1B(imu_address, 0x38, 0x01) == 2) {	//Enable interrupt when new data to be read is ready
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	while(I2C_Write_Blocking_1B(imu_address, 0x1A, 0x1) == 2) {		//Set low pass filter to 184 Hz for gyro and 188 Hz for acc (randomly chosen)
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	while(I2C_Write_Blocking_1B(imu_address, 0x37, 0x30) == 2) {	//Configure interrupt behavior
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	while(I2C_Write_Blocking_1B(imu_address, 0x6B, 0x00) == 2) {	// Power management (here clock source is Internal 8MHz oscillator)
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}

	UART_PutSTR("imu: wake MPU6050\r\n");

	return 0;
}

void IMU_Read_Values()
{
	//Ask for the 14 bytes stocked after register 0x3B
	I2C_Read_nBlocking(imu_address, 0x3B, 14);
}



/*
IMU (MPU6050)
--------------
SCL&SDA
- PIO0_4/SCL
- PIO0_5/SDA5
AD0&INT
- PIO1_10/AD6/CT16B1_MAT1
- PIO1_11/AD7
 */
