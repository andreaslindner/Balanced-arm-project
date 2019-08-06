#ifndef IMU_H_
#define IMU_H_


void IMU_Read_Values();				// Read the values of IMU acc
void PININT_IRQ_HANDLER(void);		// Handler for interrupt from IMU

/* ---IMU CONFIGURATION--- */

/**
 * @brief	Init the IMU
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
uint8_t Init_IMU();

#endif
