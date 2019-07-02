#ifndef PININT_H_
#define PININT_H_

#include <chip.h>

#define GPIO_PININT					11 					// GPIO pin number mapped to PININT
#define GPIO_PININT_PORT			1 					// GPIO port number mapped to PININT
#define IOCON_PIN_ID				IOCON_PIO1_11 		// IOCON pin identifer
#define PININT_IRQ_HANDLER			PIOINT1_IRQHandler 	// GPIO interrupt IRQ function name
#define PININT_NVIC_NAME			EINT1_IRQn 			// GPIO interrupt NVIC interrupt name

void PININT_IRQ_HANDLER(void);							// Handler for IMU interrupt
void Init_PININT();										// Init config gpio pin

#endif /* PININT_H_ */
