#ifndef IMU_H_
#define IMU_H_

void Init_IMU();					//Init imu
void IMU_Read_Values();				//Read the values of IMU acc
void PININT_IRQ_HANDLER(void);		//ISR for interrupt from IMU


#endif
