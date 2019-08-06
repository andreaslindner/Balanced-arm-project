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
	while (i < (BUFF_SAMPLE_SIZE + 21)) {
		I2C_Read_Blocking(imu_address, 0x3B, 14);					//we update the values of the IMU

		if ((i > 20) && (i <= (BUFF_SAMPLE_SIZE + 20))) {			//we don't take the first 100 values

			acc_x_buff += values[0];
			acc_z_buff += values[2];
			gyro_y_buff += values[5];
		}

		if (i == (BUFF_SAMPLE_SIZE + 20)) {						//last sample, we compute the mean
			*acc_x_mean = acc_x_buff / BUFF_SAMPLE_SIZE;
			*acc_z_mean = acc_z_buff / BUFF_SAMPLE_SIZE;
			*gyro_y_mean = gyro_y_buff / BUFF_SAMPLE_SIZE;
		}
		++i;
		TIMER_Wait_Till(51);		//wait till 10 ms
		TIMER_Reset();
	}
	TIMER_Stop();
}

void calculate_offset(int16_t acc_x_offset_start, int16_t acc_z_offset_start, int16_t gyro_y_offset_start)
{
	int8_t passCounter;

	int16_t acc_x_offset = acc_x_offset_start, acc_z_offset = acc_z_offset_start, gyro_y_offset = gyro_y_offset_start;
	int16_t acc_x_mean, acc_z_mean, gyro_y_mean;

	if (set_offset(acc_x_offset, acc_z_offset, gyro_y_offset)) {	//if we fail to set the offsets, we quit the function but we print that we failed
		UART_PutSTR("TIMEOUT\n");
		return;
	}

	calculate_mean(&acc_x_mean, &acc_z_mean, &gyro_y_mean);

	acc_x_offset = - acc_x_mean / 8;			//acc offset values are on LSB sensitivity : 2048 LSB/g, acc_mean values are on LSB sensitivity : 16 384 LSB/g so we divide by 8
	acc_z_offset = (4096 - acc_z_mean) / 8;
	gyro_y_offset = - gyro_y_mean / 4;			//gyro offset values are on LSB sensitivity : 32.8 LSB/(°/s), gyro_mean values are on LSB sensitivity : 131 LSB/(°/s) so we divide by 4

	while(1) {
		if (set_offset(acc_x_offset, acc_z_offset, gyro_y_offset)) {	//if we fail to set the offsets, we quit the function but we print that we failed
			UART_PutSTR("TIMEOUT\n");
			return;
		}

		passCounter = 0;

		/* We re-calculate the mean values, to see if the precision of the offset is enough for us, if not, we loop again */

		calculate_mean(&acc_x_mean, &acc_z_mean, &gyro_y_mean);

		if (abs(acc_x_mean) <= ACC_SENSITIVITY) {
			passCounter+=1;
		} else {
			acc_x_offset -= acc_x_mean / 8;
		}
		if (abs(4096 - acc_z_mean) <= (ACC_SENSITIVITY)) {
			passCounter+=2;
		} else {
			acc_z_offset -= (acc_z_mean - 4096) / 8;
		}
		if (abs(gyro_y_mean) <= GYRO_SENSITIVITY) {
			passCounter+=4;
		} else {
			gyro_y_offset -= gyro_y_mean / 4;
		}
		/*
		UART_PutINT(passCounter);
		UART_PutSTR("\n");
		UART_PutINT(acc_x_mean);
		UART_PutSTR("\n");
		UART_PutINT(acc_z_mean);
		UART_PutSTR("\n");
		UART_PutINT(gyro_y_mean);
		UART_PutSTR("\n\n");
		*/
		if (passCounter == 7) {	//if all the tests have succeeded, we got our offset values, we break the loop
			break;
		}

	}
	/*UART_PutSTR("Cali is finished\n");*/
}

uint8_t set_offset(int16_t acc_x_off, int16_t acc_z_off, int16_t gyro_y_off)
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
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, ACC_X_OFF_L, acc_x_offsetL) == 2) {
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, ACC_Z_OFF_H, acc_z_offsetH) == 2) {
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, ACC_Z_OFF_L, acc_z_offsetL) == 2) {
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, GYRO_Y_OFF_H, gyro_y_offsetH) == 2) {
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}

	while(I2C_Write_Blocking_1B(imu_address, GYRO_Y_OFF_L, gyro_y_offsetL) == 2) {
		counter_timeout++;
		if (counter_timeout == 2){
			return 1;
		}
	}
	return 0;
}
