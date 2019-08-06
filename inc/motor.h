#ifndef MOTOR_H_
#define MOTOR_H_


/* ---MOTOR CONFIGURATION-- */

/**
 * @brief	Init both motors
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void Init_Motor();

/**
 * @brief	Start both motors
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void Motor_Start();

/**
 * @brief	Stop both motors
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void Motor_Stop();

/**
 * @brief	Change the direction of both motors
 * @param	forward : FORWARD (10) for set the direction as forward and BACKWARD (11) for backward
 * @return	Nothing
 * @note	Nothing
 */
void Motor_changeDir(uint8_t forward);


/* ---MOTOR MOVEMENT--- */

/**
 * @brief	Set both motors to move backward
 * @param	perPower : percent (not really a percent because its on PERIOD_RESET_(R or L)MOTOR and not 100, PERIOD_RESET_(R or L)MOTOR = 5V) of power desired
 * @return	Nothing
 * @note	Nothing
 */
void Motor_Backward(uint16_t perPower);

/**
 * @brief	Set both motors to move forward
 * @param	perPower : percent (not really a percent because its on PERIOD_RESET_(R or L)MOTOR and not 100, PERIOD_RESET_(R or L)MOTOR = 5V) of power desired
 * @return	Nothing
 * @note	Nothing
 */
void Motor_Forward(uint16_t perPower);

/**
 * @brief	Set the power of both motors
 * @param	motorPower : percent (not really a percent because its on PERIOD_RESET_(R or L)MOTOR and not 100, PERIOD_RESET_(R or L)MOTOR = 5V) of power desired, can be negative
 * @return	Nothing
 * @note	negative value means go forward and a positive one go backward
 */
void Motor_setPower(float motorPower);

#endif /* MOTOR_H_ */
