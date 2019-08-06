#ifndef LMOTOR_H_
#define LMOTOR_H_

#define PERIOD_RESET_LMOTOR 1600 // Number of TIMER ticks for pwm period
#define LFREQ_LOOP 1000			// Frequency of the loop

#include <stdint.h>

#define FORWARD 10
#define BACKWARD 11

/* ---MOTOR CONFIGURATION-- */

/**
 * @brief	Init left motor
 * @param	Nothing
 * @return	Nothing
 * @note	The voltage supply is controlled by the 32-bit TIMER 1
 */
void Init_lMotor();

/**
 * @brief	Start left motor
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void lMotor_Start();

/**
 * @brief	Stop left motor
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void lMotor_Stop();

/**
 * @brief	Change the direction of left motor
 * @param	forward : FORWARD (10) for set the direction as forward and BACKWARD (11) for backward
 * @return	Nothing
 * @note	Nothing
 */
void lMotor_changeDir(uint8_t forward);


/* ---MOTOR MOVEMENT--- */

/**
 * @brief	Set left motor to move backward
 * @param	perPower : percent (not really a percent because its on PERIOD_LRESET_MOTOR and not 100, PERIOD_RESET_LMOTOR = 5V) of power desired
 * @return	Nothing
 * @note	Nothing
 */
void lMotor_Backward(uint16_t perPower);

/**
 * @brief	Set left motor to move forward
 * @param	perPower : percent (not really a percent because its on PERIOD_RESET_LMOTOR and not 100, PERIOD_RESET_LMOTOR = 5V) of power desired
 * @return	Nothing
 * @note	Nothing
 */
void lMotor_Forward(uint16_t perPower);

/**
 * @brief	check if motor is set as forward or backward
 * @param	Nothing
 * @return	return 1 if the motor's direction is set as forward and 0 if it is set as backward
 * @note	Nothing
 */
uint8_t lMotor_isForward();

#endif /* LMOTOR_H_ */
