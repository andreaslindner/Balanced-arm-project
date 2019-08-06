#ifndef PININT_H_
#define PININT_H_

#include <chip.h>

#define RAD_TO_DEG		57.29578
#define ALPHA			0.9								//change this value to change the filter : increase means give more power to gyro and decrease more power to acc
#define KP				40								//coefficient of the proportional term (based on the current error)
#define KI				40								//coefficient of the integral term (based on the accumulated error)
#define KD				0.05							//coefficient of the derivate term (based on the possible future error)

#define GPIO_PININT					11 					// GPIO pin number mapped to PININT
#define GPIO_PININT_PORT			1 					// GPIO port number mapped to PININT
#define IOCON_PIN_ID				IOCON_PIO1_11 		// IOCON pin identifer
#define PININT_IRQ_HANDLER			PIOINT1_IRQHandler 	// GPIO interrupt IRQ function name
#define PININT_NVIC_NAME			EINT1_IRQn 			// GPIO interrupt NVIC interrupt name


/**
 * @brief	Handler of the pinint interrupt #Control loop
 * @param	Nothing
 * @return	Nothing
 * @note	This handler is the control loop, responsible of balancing the robot, it measures the current angle and set voltage to the motor in function of this angle
 */
void PININT_IRQ_HANDLER(void);

/**
 * @brief	Enable GPIO pin interrupt to be triggered from interrupt triggered from IMU
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void Init_PININT();

#endif /* PININT_H_ */
