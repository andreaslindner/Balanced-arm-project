#ifndef IMU_H_
#define IMU_H_

uint8_t Init_IMU();					// Init imu
void IMU_Read_Values();				// Read the values of IMU acc
void PININT_IRQ_HANDLER(void);		// Handler for interrupt from IMU


#endif
