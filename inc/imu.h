#ifndef IMU_H_
#define IMU_H_



/* ---IMU CONFIGURATION--- */

/**
 * @brief	Init the IMU, configure it (interrupt configuration is done in Init_PININT()
 * @param	Nothing
 * @return	Nothing
 * @note	See the source code for more details about the configuration
 */
uint8_t Init_IMU();


/* ---IMU READ--- */

/**
 * @brief	Init a read of the values measured by the IMU
 * @param	Nothing
 * @return	Nothing
 * @note	Not blocking read
 */
void IMU_Read_Values();

#endif
