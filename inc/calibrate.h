#ifndef CALIBRATE_H_
#define CALIBRATE_H_

#define BUFF_SAMPLE_SIZE 		400
#define ACC_SENSITIVITY			8
#define GYRO_SENSITIVITY		4

#define ACC_X_OFF_H 			0x06
#define ACC_X_OFF_L				0x07
#define ACC_Z_OFF_H				0x0A
#define ACC_Z_OFF_L				0x0B
#define GYRO_Y_OFF_H			0x15
#define GYRO_Y_OFF_L			0x16

void calculate_mean(int16_t *acc_x_mean, int16_t *acc_z_mean, int16_t *gyro_x_mean); 			//Calculate the mean for acc_x, acc_z and gyro_x values
void calculate_offset();																		// Calculate the offset for acc_x, acc_z, gyro_x7
void set_offset(int16_t acc_y_off, int16_t acc_z_off, int16_t gyro_x_off);						//Set the offset

#endif /* CALIBRATE_H_ */
