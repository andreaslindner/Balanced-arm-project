#ifndef CALIBRATE_H_
#define CALIBRATE_H_

#define BUFF_SAMPLE_SIZE 		100
#define ACC_SENSITIVITY			8
#define GYRO_SENSITIVITY		4

#define ACC_X_OFF				-2360
#define ACC_Z_OFF				1076
#define GYRO_Y_OFF				17

#define ACC_X_OFF_H 			0x06
#define ACC_X_OFF_L				0x07
#define ACC_Z_OFF_H				0x0A
#define ACC_Z_OFF_L				0x0B
#define GYRO_Y_OFF_H			0x15
#define GYRO_Y_OFF_L			0x16

void calculate_mean(int16_t *acc_x_mean, int16_t *acc_z_mean, int16_t *gyro_x_mean); 								//Calculate the mean for acc_x, acc_z and gyro_y values
void calculate_offset(int16_t acc_x_offset_start, int16_t acc_z_offset_start, int16_t gyro_y_offset_start);			// Calculate and set the offset for acc_x, acc_z, gyro_y
uint8_t set_offset(int16_t acc_x_off, int16_t acc_z_off, int16_t gyro_x_off);										//Set the offset

#endif /* CALIBRATE_H_ */
