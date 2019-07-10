#ifndef CALIBRATE_H_
#define CALIBRATE_H_

#define BUFF_SAMPLE_SIZE 		100
#define ACC_SENSITIVITY			4
#define GYRO_SENSITIVITY			1

#define ACC_Y_OFF_H 			0x08
#define ACC_Y_OFF_L				0x09
#define ACC_Z_OFF_H				0x0A
#define ACC_Z_OFF_L				0x0B
#define GYRO_X_OFF_H			0x13
#define GYRO_X_OFF_L			0x14

void calculate_mean(int16_t *acc_x_mean, int16_t *acc_z_mean, int16_t *gyro_x_mean); 			//Calculate the mean for acc_x, acc_z and gyro_x values
void calculate_offset();																		// Calculate the offset for acc_x, acc_z, gyro_x

#endif /* CALIBRATE_H_ */
