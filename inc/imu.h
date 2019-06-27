#ifndef IMU_H_
#define IMU_H_

uint32_t Init_IMU();									//Init imu
void imu_read_values();					//Read the values of IMU acc
void PININT_IRQ_HANDLER(void);							//ISR for interrupt from IMU


#endif
