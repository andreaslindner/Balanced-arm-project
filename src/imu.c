#include <LPC11xx.h>
#include <uart.h>
#include <i2c.h>

const uint8_t imu_address = 0x68;
extern volatile uint16_t values[7];

uint8_t Init_IMU()
{
	uint8_t counter_timeout = 0;
	//Init AD0, pull down valuesistor
	LPC_IOCON->PIO1_10  &= ~0x1F;
	LPC_IOCON->PIO1_10  |= 0x08;

	//Init i2c bus
	I2CInit(I2CMASTER);

	while(I2C_Write_Blocking_1B(imu_address, 0x19, 0x04) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	while(I2C_Write_Blocking_1B(imu_address, 0x1B, 0x10) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	while(I2C_Write_Blocking_1B(imu_address, 0x1C, 0x10) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	while(I2C_Write_Blocking_1B(imu_address, 0x38, 0x01) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	while(I2C_Write_Blocking_1B(imu_address, 0x1A, 0x02) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	while(I2C_Write_Blocking_1B(imu_address, 0x37, 0x30) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	while(I2C_Write_Blocking_1B(imu_address, 0x6B, 0x00) == 2) {
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
- PIO0_5/SDA
AD0&INT
- PIO1_10/AD6/CT16B1_MAT1
- PIO1_11/AD7
 */
