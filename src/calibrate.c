#include <chip.h>
#include <calibrate.h>
#include <i2c.h>
#include <timer.h>
#include <uart.h>
#include <stdlib.h>

extern volatile int16_t values[7];
extern const uint8_t imu_address;

void calculate_mean(int16_t *acc_y_mean, int16_t *acc_z_mean, int16_t *gyro_x_mean)
{
	int16_t i = 0;
	int32_t acc_y_buff = 0, acc_z_buff = 0, gyro_x_buff = 0;

	Init_TIMER();
	TIMER_Start();
	while (i < (BUFF_SAMPLE_SIZE + 101)) {

		I2C_Read_Blocking(imu_address, 0x3B, 14);					//we update the values of the IMU

		if ((i > 100) && (i <= (BUFF_SAMPLE_SIZE + 100))) {			//we don't take the first 100 values

			acc_y_buff += values[1];
			acc_z_buff += values[2];
			gyro_x_buff += values[4];
		}

		if (i == (BUFF_SAMPLE_SIZE + 100)) {						// last sample, we compute the mean
			*acc_y_mean = acc_y_buff / BUFF_SAMPLE_SIZE;
			*acc_z_mean = acc_z_buff / BUFF_SAMPLE_SIZE;
			*gyro_x_mean = gyro_x_buff / BUFF_SAMPLE_SIZE;
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

	int8_t acc_y_offsetL = 0, acc_y_offsetH = 0, acc_z_offsetL = 0, acc_z_offsetH = 0, gyro_x_offsetL = 0, gyro_x_offsetH = 0;
	int16_t acc_y_offset = 0, acc_z_offset = 0, gyro_x_offset = 0;
	int16_t acc_y_mean, acc_z_mean, gyro_x_mean;

	counter_timeout = 0;
	/*
	while(I2C_Write_Blocking_1B(imu_address, ACC_Y_OFF_H, 0) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, ACC_Y_OFF_L, 0) == 2) {
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

	while(I2C_Write_Blocking_1B(imu_address, GYRO_X_OFF_H, 0) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, GYRO_X_OFF_L, 0) == 2) {
		UART_PutSTR("Timeout on writing into the IMU\r\n");
		counter_timeout++;
		if (counter_timeout == 2){
			UART_PutSTR("Calibration failed\r\n");
			return;
		}
	}
	*/
	calculate_mean(&acc_y_mean, &acc_z_mean, &gyro_x_mean);

	acc_y_offset = - acc_y_mean / 2;				// acc offset values are on LSB sensitivity : 2048 LSB/g, acc_mean values are on LSB sensitivity : 4096 LSB/g so we divide by 2
	acc_z_offset = (4096 - acc_z_mean) / 2;
	gyro_x_offset = - gyro_x_mean / 2;			// gyro offset values are on LSB sensitivity : 32.8 LSB/(°/s), gyro_mean values are on LSB sensitivity : 65.5 LSB/(°/s) so we divide by 2

	while(1) {

		acc_y_offsetH = acc_y_offset >> 8;
		acc_y_offsetL = acc_y_offset & 0x00FF;
		acc_z_offsetH = acc_z_offset >> 8;
		acc_z_offsetL = acc_z_offset & 0x00FF;
		gyro_x_offsetH = gyro_x_offset >> 8;
		gyro_x_offsetL = gyro_x_offset & 0x00FF;

		/* We write to the IMU to give him our current values for the offsets */

		counter_timeout = 0;

		while(I2C_Write_Blocking_1B(imu_address, ACC_Y_OFF_H, acc_y_offsetH) == 2) {
			UART_PutSTR("Timeout on writing into the IMU\r\n");
			counter_timeout++;
			if (counter_timeout == 2){
				UART_PutSTR("Calibration failed\r\n");
				return;
			}
		}

		while(I2C_Write_Blocking_1B(imu_address, ACC_Y_OFF_L, acc_y_offsetL) == 2) {
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

		while(I2C_Write_Blocking_1B(imu_address, GYRO_X_OFF_H, gyro_x_offsetH) == 2) {
			UART_PutSTR("Timeout on writing into the IMU\r\n");
			counter_timeout++;
			if (counter_timeout == 2){
				UART_PutSTR("Calibration failed\r\n");
				return;
			}
		}

		while(I2C_Write_Blocking_1B(imu_address, GYRO_X_OFF_L, gyro_x_offsetL) == 2) {
			UART_PutSTR("Timeout on writing into the IMU\r\n");
			counter_timeout++;
			if (counter_timeout == 2){
				UART_PutSTR("Calibration failed\r\n");
				return;
			}
		}

		passCounter = 0;

		/* We re-calculate the mean values, to see if the precision of the offset is enough for us, if not, we loop again */

		calculate_mean(&acc_y_mean, &acc_z_mean, &gyro_x_mean);

		if (abs(acc_y_mean) <= ACC_SENSITIVITY) {
			passCounter++;
		} else {
			acc_y_offset -= acc_y_mean / 2;
		}
		if (abs(4096 - acc_z_mean) <= (ACC_SENSITIVITY)) {
			passCounter++;
		} else {
			acc_z_offset -= (acc_z_mean - 4096) / 2;
		}
		if (abs(gyro_x_mean) <= GYRO_SENSITIVITY) {
			passCounter++;
		} else {
			gyro_x_offset -= gyro_x_mean / 2;
		}

		if (passCounter == 3) {	// if all the tests have succeeded, we got our offset values, we break the loop
			break;
		}
	}

	UART_PutSTR("Offset values are set\r\n");
}
