#include <chip.h>
#include <calibrate.h>
#include <i2c.h>
#include <timer.h>
#include <uart.h>
#include <stdlib.h>

extern volatile int16_t values[7];
extern const uint8_t imu_address;

void calculate_mean(int16_t *acc_x_mean, int16_t *acc_z_mean, int16_t *gyro_y_mean)
{
	int16_t i = 0;
	int32_t acc_x_buff = 0, acc_z_buff = 0, gyro_y_buff = 0;

	Init_TIMER();
	TIMER_Start();
	while (i < (BUFF_SAMPLE_SIZE + 101)) {

		I2C_Read_Blocking(imu_address, 0x3B, 14);					//we update the values of the IMU

		if ((i > 100) && (i <= (BUFF_SAMPLE_SIZE + 100))) {			//we don't take the first 100 values

			acc_x_buff += values[0];
			acc_z_buff += values[2];
			gyro_y_buff += values[5];
		}

		if (i == (BUFF_SAMPLE_SIZE + 100)) {						// last sample, we compute the mean
			*acc_x_mean = acc_x_buff / BUFF_SAMPLE_SIZE;
			*acc_z_mean = acc_z_buff / BUFF_SAMPLE_SIZE;
			*gyro_y_mean = gyro_y_buff / BUFF_SAMPLE_SIZE;
		}
		++i;
		TIMER_Wait_Till(100);		//wait till 10 ms
		TIMER_Reset();
	}
	TIMER_Stop();
}

void calculate_offset()
{
	int8_t passCounter;
	int8_t counter_timeout;

	int8_t acc_x_offsetL = 0, acc_x_offsetH = 0, acc_z_offsetL = 0, acc_z_offsetH = 0, gyro_y_offsetL = 0, gyro_y_offsetH = 0;
	int16_t acc_x_offset = 0, acc_z_offset = 0, gyro_y_offset = 0;
	int16_t acc_x_mean, acc_z_mean, gyro_y_mean;

	counter_timeout = 0;

	while(I2C_Write_Blocking_1B(imu_address, ACC_X_OFF_H, 0) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, ACC_X_OFF_L, 0) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, ACC_Z_OFF_H, 0) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, ACC_Z_OFF_L, 0) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, GYRO_Y_OFF_H, 0) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, GYRO_Y_OFF_L, 0) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	calculate_mean(&acc_x_mean, &acc_z_mean, &gyro_y_mean);

	acc_x_offset = - acc_x_mean / 8;				// acc offset values are on LSB sensitivity : 2048 LSB/g, acc_mean values are on LSB sensitivity : 16 384 LSB/g so we divide by 8
	acc_z_offset = (4096 - acc_z_mean) / 8;
	gyro_y_offset = - gyro_y_mean / 4;			// gyro offset values are on LSB sensitivity : 32.8 LSB/(°/s), gyro_mean values are on LSB sensitivity : 131 LSB/(°/s) so we divide by 4

	while(1) {

		acc_x_offsetH = acc_x_offset >> 8;
		acc_x_offsetL = acc_x_offset & 0x00FF;
		acc_z_offsetH = acc_z_offset >> 8;
		acc_z_offsetL = acc_z_offset & 0x00FF;
		gyro_y_offsetH = gyro_y_offset >> 8;
		gyro_y_offsetL = gyro_y_offset & 0x00FF;

		/* We write to the IMU to give him our current values for the offsets */

		counter_timeout = 0;

		while(I2C_Write_Blocking_1B(imu_address, ACC_X_OFF_H, acc_x_offsetH) == 2) {
			UART_PutSTR("Timeout on writing into the IMU\r\n");
			counter_timeout++;
			if (counter_timeout == 2){
				UART_PutSTR("Calibration failed\r\n");
				return;
			}
		}

		while(I2C_Write_Blocking_1B(imu_address, ACC_X_OFF_L, acc_x_offsetL) == 2) {
			UART_PutSTR("Timeout on writing into the IMU\r\n");
			counter_timeout++;
			if (counter_timeout == 2){
				UART_PutSTR("Calibration failed\r\n");
				return;
			}
		}

		while(I2C_Write_Blocking_1B(imu_address, ACC_Z_OFF_H, acc_z_offsetH) == 2) {
			UART_PutSTR("Timeout on writing into the IMU\r\n");
			counter_timeout++;
			if (counter_timeout == 2){
				UART_PutSTR("Calibration failed\r\n");
				return;
			}
		}

		while(I2C_Write_Blocking_1B(imu_address, ACC_Z_OFF_L, acc_z_offsetL) == 2) {
			UART_PutSTR("Timeout on writing into the IMU\r\n");
			counter_timeout++;
			if (counter_timeout == 2){
				UART_PutSTR("Calibration failed\r\n");
				return;
			}
		}

		while(I2C_Write_Blocking_1B(imu_address, GYRO_Y_OFF_H, gyro_y_offsetH) == 2) {
			UART_PutSTR("Timeout on writing into the IMU\r\n");
			counter_timeout++;
			if (counter_timeout == 2){
				UART_PutSTR("Calibration failed\r\n");
				return;
			}
		}

		while(I2C_Write_Blocking_1B(imu_address, GYRO_Y_OFF_L, gyro_y_offsetL) == 2) {
			UART_PutSTR("Timeout on writing into the IMU\r\n");
			counter_timeout++;
			if (counter_timeout == 2){
				UART_PutSTR("Calibration failed\r\n");
				return;
			}
		}

		passCounter = 0;

		/* We re-calculate the mean values, to see if the precision of the offset is enough for us, if not, we loop again */

		calculate_mean(&acc_x_mean, &acc_z_mean, &gyro_y_mean);

		if (abs(acc_x_mean) <= ACC_SENSITIVITY) {
			passCounter++;
		} else {
			acc_x_offset -= acc_x_mean / 8;
		}
		if (abs(4096 - acc_z_mean) <= (ACC_SENSITIVITY)) {
			passCounter++;
		} else {
			acc_z_offset -= (acc_z_mean - 4096) / 8;
		}
		if (abs(gyro_y_mean) <= GYRO_SENSITIVITY) {
			passCounter++;
		} else {
			gyro_y_offset -= gyro_y_mean / 4;
		}

		if (passCounter == 3) {	// if all the tests have succeeded, we got our offset values, we break the loop
			break;
		}
	}

	UART_PutSTR("Offset values are set\r\n");
}

void set_offset(int16_t acc_x_off, int16_t acc_z_off, int16_t gyro_y_off)
{
	uint8_t counter_timeout;


	int8_t acc_x_offsetH = acc_x_off >> 8;
	int8_t acc_x_offsetL = acc_x_off & 0x00FF;
	int8_t acc_z_offsetH = acc_z_off >> 8;
	int8_t acc_z_offsetL = acc_z_off & 0x00FF;
	int8_t gyro_y_offsetH = gyro_y_off >> 8;
	int8_t gyro_y_offsetL = gyro_y_off & 0x00FF;

	/* We write to the IMU to give him our current values for the offsets */

	counter_timeout = 0;

	while(I2C_Write_Blocking_1B(imu_address, ACC_X_OFF_H, acc_x_offsetH) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, ACC_X_OFF_L, acc_x_offsetL) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, ACC_Z_OFF_H, acc_z_offsetH) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, ACC_Z_OFF_L, acc_z_offsetL) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, GYRO_Y_OFF_H, gyro_y_offsetH) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, GYRO_Y_OFF_L, gyro_y_offsetL) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}
}
