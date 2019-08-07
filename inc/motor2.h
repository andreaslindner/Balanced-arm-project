#ifndef MOTOR2_H_
#define MOTOR2_H_

#include <stdint.h>
#include <chip.h>


/* ---MOTORS DEF--- */

#define MOTORS_PERIOD_RESET									1600 /* 1600 ticks before reset */
#define MOTORS_FREQ_LOOP									1000 /* 1ms */
#define GPIO_BREAK											1
#define GPIO_FREE_WHEELS									0
#define BACKWARD											11
#define FORWARD												10

/* ---LMOTOR CONFIG--- */

#define PWM_LMOTOR_PIN_NUMBER								8
#define PWM_LMOTOR_PIN_PORT									0
#define PWM_LMOTOR_PIN_ID									IOCON_PIO0_8
#define PWM_LMOTOR_PIN_FUNC 								2
#define PWM_LMOTOR_PIN_MATCH_NUMBER							0
#define PWM_LMOTOR_PIN_RESET_NUMBER							3

#define GPIO_LMOTOR_FORWARD_PIN_NUMBER						9
#define GPIO_LMOTOR_FORWARD_PIN_PORT						0
#define GPIO_LMOTOR_FORWARD_PIN_ID							IOCON_PIO0_9
#define GPIO_LMOTOR_FORWARD_PIN_FUNC						0

#define GPIO_LMOTOR_BACKWARD_PIN_NUMBER						1
#define GPIO_LMOTOR_BACKWARD_PIN_PORT						3
#define GPIO_LMOTOR_BACKWARD_PIN_ID							IOCON_PIO3_1
#define GPIO_LMOTOR_BACKWARD_PIN_FUNC						0

#define LMOTOR_TIMER										LPC_TIMER16_0

/* ---RMOTOR CONFIG--- */

#define PWM_RMOTOR_PIN_NUMBER								1
#define PWM_RMOTOR_PIN_PORT									1
#define PWM_RMOTOR_PIN_ID									IOCON_PIO1_1
#define PWM_RMOTOR_PIN_FUNC 								3
#define PWM_RMOTOR_PIN_MATCH_NUMBER							0
#define PWM_RMOTOR_PIN_RESET_NUMBER							3

#define GPIO_RMOTOR_BACKWARD_PIN_NUMBER						2
#define GPIO_RMOTOR_BACKWARD_PIN_PORT						1
#define GPIO_RMOTOR_BACKWARD_PIN_ID							IOCON_PIO1_2
#define GPIO_RMOTOR_BACKWARD_PIN_FUNC						1

#define GPIO_RMOTOR_FORWARD_PIN_NUMBER						2
#define GPIO_RMOTOR_FORWARD_PIN_PORT						3
#define GPIO_RMOTOR_FORWARD_PIN_ID							IOCON_PIO3_2
#define GPIO_RMOTOR_FORWARD_PIN_FUNC						0

#define RMOTOR_TIMER										LPC_TIMER32_1

/**
 * @brief	Init both motors
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void Init_Motors();

/**
 * @brief	Stop both motors
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void Motors_Stop();

/**
 * @brief	Change the direction of the motors
 * @param	direction : FORWARD to go forward, BACKWARD to go backward
 * @return	Nothing
 * @note	Doesn't change pwm value
 */
void Motors_changeDir(uint8_t direction);

/**
 * @brief	Check if the motors are configured as forward
 * @param	Nothing
 * @return	1 if motors are set as forward, otherwise 0
 * @note	Nothing
 */
uint8_t Motors_areForward();

/**
 * @brief	Check if the motors are configured as backward
 * @param	Nothing
 * @return	1 if motors are set as backward, otherwise 0
 * @note	Nothing
 */
uint8_t Motors_areBackward();

/**
 * @brief	Move forward
 * @param	power : pwm value
 * @return	Nothing
 * @note	Nothing
 */
void Motors_Forward(uint16_t power);

/**
 * @brief	Move backward
 * @param	power : pwm value
 * @return	Nothing
 * @note	Nothing
 */
void Motors_Backward(uint16_t power);

/**
 * @brief	Start the motors
 * @param	Nothing
 * @return	Nothing
 * @note	Need to be called after Init_Motors() in order to use Motors_Forward / Motors_Backward / Motors_changeDir
 */
void Motors_Start();

/**
 * @brief	Move the wheels backward or forward
 * @param	powerFloat : power to give to both motors
 * @return	Nothing
 * @note	if powerFloat is negative it will go forward, overwhise backward
 */
void Motors_Move(float powerFloat);

#endif /* MOTOR2_H_ */
