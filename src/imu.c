#include <LPC11xx.h>
#include <uart.h>
#include <i2c.h>

const uint8_t imu_address = 0x68;
extern volatile uint16_t values[7];

uint32_t Init_IMU()
{
	//Init AD0, pull down valuesistor
	LPC_IOCON->PIO1_10  &= ~0x1F;
	LPC_IOCON->PIO1_10  |= 0x08;

	//Init i2c bus
	I2CInit(I2CMASTER);

	//Configuring sample rate divider
	while (twi_write(imu_address, 0x19, 0x09) != 0);
	//Configuring gyro range to +-1000deg/s
	while (twi_write(imu_address, 0x1B, 0x10) != 0);
	//Configuring acc range to +-8g
	while (twi_write(imu_address, 0x1C, 0x10) != 0);
	//Enable data-read interrupt
	while (twi_write(imu_address, 0x38, 0x01) != 0);
	//Chose f0 for low filter (here 94Hz)
	while (twi_write(imu_address, 0x1A, 0x02) != 0);
	//Config INT pin
	while (twi_write(imu_address, 0x37, 0x30) != 0);

	//UART_PutSTR("imu: wake MPU6050\r\n");

	//Configure power management
	while(twi_write(imu_address, 0x6B, 0x00 != 0));

	return(0);
}

void imu_read_values()
{
	//Ask for the 14 bytes stocked after register 0x3B
	twi_read_n(imu_address, 0x3B, 14);
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
