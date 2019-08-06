#ifndef RMOTOR_H_
#define RMOTOR_H_

#define PERIOD_RESET_RMOTOR 1600 	// Number of TIMER ticks for pwm period
#define RFREQ_LOOP 1000			// Frequency of the loop

#include <stdint.h>

#define FORWARD 10
#define BACKWARD 11

/* ---MOTOR CONFIGURATION-- */

/**
 * @brief	Init right motor
 * @param	Nothing
 * @return	Nothing
 * @note	The voltage supply is controlled by the 16-bit TIMER 0
 */
void Init_rMotor();

/**
 * @brief	Start right motor
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void rMotor_Start();

/**
 * @brief	Stop right motor
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void rMotor_Stop();

/**
 * @brief	Change the direction of right motor
 * @param	forward : FORWARD (10) for set the direction as forward and BACKWARD (11) for backward
 * @return	Nothing
 * @note	Nothing
 */
void rMotor_changeDir(uint8_t forward);


/* ---MOTOR MOVEMENT--- */

/**
 * @brief	Set right motor to move backward
 * @param	perPower : percent (not really a percent because its on PERIOD_RESET_RMOTOR and not 100, PERIOD_RESET_RMOTOR = 5V) of power desired
 * @return	Nothing
 * @note	Nothing
 */
void rMotor_Backward(uint16_t perPower);

/**
 * @brief	Set right motor to move forward
 * @param	perPower : percent (not really a percent because its on PERIOD_RESET_RMOTOR and not 100, PERIOD_RESET_RMOTOR = 5V) of power desired
 * @return	Nothing
 * @note	Nothing
 */
void rMotor_Forward(uint16_t perPower);

/**
 * @brief	check if motor is set as forward or backward
 * @param	Nothing
 * @return	return 1 if the motor's direction is set as forward and 0 if it is set as backward
 * @note	Nothing
 */
uint8_t rMotor_isForward();

#endif /* RMOTOR_H_ */
