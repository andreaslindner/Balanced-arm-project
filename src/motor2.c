#include <motor2.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <uart.h>

volatile uint8_t motors_on = 0;

void Init_Motors()
{
	uint8_t i;

	/* Change direction of GPIO and PWM pins */

		/* L */
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_LMOTOR_FORWARD_PIN_PORT, GPIO_LMOTOR_FORWARD_PIN_NUMBER);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_LMOTOR_BACKWARD_PIN_PORT, GPIO_LMOTOR_BACKWARD_PIN_NUMBER);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, PWM_LMOTOR_PIN_PORT, PWM_LMOTOR_PIN_NUMBER);

		/* R */
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_RMOTOR_FORWARD_PIN_PORT, GPIO_RMOTOR_FORWARD_PIN_NUMBER);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, GPIO_RMOTOR_BACKWARD_PIN_PORT, GPIO_RMOTOR_BACKWARD_PIN_NUMBER);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, PWM_RMOTOR_PIN_PORT, PWM_RMOTOR_PIN_NUMBER);

	/* Change pin function */

		/* L */
	Chip_IOCON_PinMuxSet(LPC_IOCON, GPIO_LMOTOR_FORWARD_PIN_ID, GPIO_LMOTOR_FORWARD_PIN_FUNC);
	Chip_IOCON_PinMuxSet(LPC_IOCON, GPIO_LMOTOR_BACKWARD_PIN_ID, GPIO_LMOTOR_BACKWARD_PIN_FUNC);
	Chip_IOCON_PinMuxSet(LPC_IOCON, PWM_LMOTOR_PIN_ID, PWM_LMOTOR_PIN_FUNC);

		/* R */
	Chip_IOCON_PinMuxSet(LPC_IOCON, GPIO_RMOTOR_FORWARD_PIN_ID, GPIO_RMOTOR_FORWARD_PIN_FUNC);
	Chip_IOCON_PinMuxSet(LPC_IOCON, GPIO_RMOTOR_BACKWARD_PIN_ID, GPIO_RMOTOR_BACKWARD_PIN_FUNC);
	Chip_IOCON_PinMuxSet(LPC_IOCON, PWM_RMOTOR_PIN_ID, PWM_RMOTOR_PIN_FUNC);

	/* Initialize TIMER used to produce PWM */

		/* L */
	Chip_TIMER_Init(LMOTOR_TIMER);
	Chip_TIMER_PrescaleSet(LMOTOR_TIMER, Chip_Clock_GetSystemClockRate() / (MOTORS_PERIOD_RESET * MOTORS_FREQ_LOOP));
	Chip_TIMER_Disable(LMOTOR_TIMER);
	Chip_TIMER_Reset(LMOTOR_TIMER);

		/* R */
	Chip_TIMER_Init(RMOTOR_TIMER);
	Chip_TIMER_PrescaleSet(RMOTOR_TIMER, Chip_Clock_GetSystemClockRate() / (MOTORS_PERIOD_RESET * MOTORS_FREQ_LOOP));
	Chip_TIMER_Disable(RMOTOR_TIMER);
	Chip_TIMER_Reset(RMOTOR_TIMER);

	/* Set the reset match of the TIMER */
	for (i = 0; i < 3; ++i) {

		/* L */

		Chip_TIMER_ResetOnMatchDisable(LMOTOR_TIMER, i);



		/* R */

		Chip_TIMER_ResetOnMatchDisable(RMOTOR_TIMER, i);

	}
	Chip_TIMER_SetMatch(LMOTOR_TIMER, PWM_LMOTOR_PIN_RESET_NUMBER, MOTORS_PERIOD_RESET);
	Chip_TIMER_ResetOnMatchEnable(LMOTOR_TIMER, PWM_LMOTOR_PIN_RESET_NUMBER);
	Chip_TIMER_SetMatch(RMOTOR_TIMER, PWM_RMOTOR_PIN_RESET_NUMBER, MOTORS_PERIOD_RESET);
	Chip_TIMER_ResetOnMatchEnable(RMOTOR_TIMER, PWM_RMOTOR_PIN_RESET_NUMBER);

	/* Turn off the motor, block the wheel */

		/* L */
	Chip_GPIO_SetPinState(LPC_GPIO, GPIO_LMOTOR_FORWARD_PIN_PORT, GPIO_LMOTOR_FORWARD_PIN_NUMBER, GPIO_BREAK);
	Chip_GPIO_SetPinState(LPC_GPIO, GPIO_LMOTOR_BACKWARD_PIN_PORT, GPIO_LMOTOR_BACKWARD_PIN_NUMBER, GPIO_BREAK);
	Chip_TIMER_SetMatch(LMOTOR_TIMER, PWM_LMOTOR_PIN_MATCH_NUMBER, MOTORS_PERIOD_RESET);

		/* R */
	Chip_GPIO_SetPinState(LPC_GPIO, GPIO_RMOTOR_FORWARD_PIN_PORT, GPIO_RMOTOR_FORWARD_PIN_NUMBER, GPIO_BREAK);
	Chip_GPIO_SetPinState(LPC_GPIO, GPIO_RMOTOR_BACKWARD_PIN_PORT, GPIO_RMOTOR_BACKWARD_PIN_NUMBER, GPIO_BREAK);
	Chip_TIMER_SetMatch(RMOTOR_TIMER, PWM_RMOTOR_PIN_MATCH_NUMBER, MOTORS_PERIOD_RESET);

	motors_on = 0;
}

void Motors_Stop()
{
	/* Disable TIMERs */

		/* L */
	Chip_TIMER_Disable(LMOTOR_TIMER);

		/* R */
	Chip_TIMER_Disable(RMOTOR_TIMER);

	/* Turn of the motor, block the wheel */

		/* L */
	Chip_GPIO_SetPinState(LPC_GPIO, GPIO_LMOTOR_FORWARD_PIN_PORT, GPIO_LMOTOR_FORWARD_PIN_NUMBER, GPIO_BREAK);
	Chip_GPIO_SetPinState(LPC_GPIO, GPIO_LMOTOR_BACKWARD_PIN_PORT, GPIO_LMOTOR_BACKWARD_PIN_NUMBER, GPIO_BREAK);
	Chip_TIMER_SetMatch(LMOTOR_TIMER, PWM_LMOTOR_PIN_MATCH_NUMBER, MOTORS_PERIOD_RESET);

		/* R */
	Chip_GPIO_SetPinState(LPC_GPIO, GPIO_RMOTOR_FORWARD_PIN_PORT, GPIO_RMOTOR_FORWARD_PIN_NUMBER, GPIO_BREAK);
	Chip_GPIO_SetPinState(LPC_GPIO, GPIO_RMOTOR_BACKWARD_PIN_PORT, GPIO_RMOTOR_BACKWARD_PIN_NUMBER, GPIO_BREAK);
	Chip_TIMER_SetMatch(RMOTOR_TIMER, PWM_RMOTOR_PIN_MATCH_NUMBER, MOTORS_PERIOD_RESET);

	motors_on = 0;
}

void Motors_changeDir(uint8_t direction)
{
	if (motors_on) {
		if (direction == FORWARD) {

			/* Change GPIO pins value */

				/* L */
			Chip_GPIO_SetPinState(LPC_GPIO, GPIO_LMOTOR_BACKWARD_PIN_PORT, GPIO_LMOTOR_BACKWARD_PIN_NUMBER, false);
			Chip_GPIO_SetPinState(LPC_GPIO, GPIO_LMOTOR_FORWARD_PIN_PORT, GPIO_LMOTOR_FORWARD_PIN_NUMBER, true);

				/* R */
			Chip_GPIO_SetPinState(LPC_GPIO, GPIO_RMOTOR_BACKWARD_PIN_PORT, GPIO_RMOTOR_BACKWARD_PIN_NUMBER, false);
			Chip_GPIO_SetPinState(LPC_GPIO, GPIO_RMOTOR_FORWARD_PIN_PORT, GPIO_RMOTOR_FORWARD_PIN_NUMBER, true);

		} else if (direction == BACKWARD) {

			/* Change GPIO pins value */

				/* L */
			Chip_GPIO_SetPinState(LPC_GPIO, GPIO_LMOTOR_FORWARD_PIN_PORT, GPIO_LMOTOR_FORWARD_PIN_NUMBER, false);
			Chip_GPIO_SetPinState(LPC_GPIO, GPIO_LMOTOR_BACKWARD_PIN_PORT, GPIO_LMOTOR_BACKWARD_PIN_NUMBER, true);

				/* R */
			Chip_GPIO_SetPinState(LPC_GPIO, GPIO_RMOTOR_FORWARD_PIN_PORT, GPIO_RMOTOR_FORWARD_PIN_NUMBER, false);
			Chip_GPIO_SetPinState(LPC_GPIO, GPIO_RMOTOR_BACKWARD_PIN_PORT, GPIO_RMOTOR_BACKWARD_PIN_NUMBER, true);
		}
	}
}

uint8_t Motors_areForward()
{
	return(Chip_GPIO_GetPinState(LPC_GPIO, GPIO_RMOTOR_FORWARD_PIN_PORT, GPIO_RMOTOR_FORWARD_PIN_NUMBER) && (Chip_GPIO_GetPinState(LPC_GPIO, GPIO_LMOTOR_FORWARD_PIN_PORT, GPIO_LMOTOR_FORWARD_PIN_NUMBER)));
}

uint8_t Motors_areBackward()
{
	return(Chip_GPIO_GetPinState(LPC_GPIO, GPIO_RMOTOR_BACKWARD_PIN_PORT, GPIO_RMOTOR_BACKWARD_PIN_NUMBER) && (Chip_GPIO_GetPinState(LPC_GPIO, GPIO_LMOTOR_BACKWARD_PIN_PORT, GPIO_LMOTOR_BACKWARD_PIN_NUMBER)));
}

void Motors_Forward(uint16_t power)
{
	if (motors_on) {
		/* Change direction of the motors if it's not forward */
		if (!Motors_areForward()) {
			Motors_changeDir(FORWARD);
		}
		/* change PWM output */
		if ((0 <= power) && (power <= MOTORS_PERIOD_RESET )) {
			/* L */
			Chip_TIMER_SetMatch(LMOTOR_TIMER, PWM_LMOTOR_PIN_MATCH_NUMBER, MOTORS_PERIOD_RESET - power);

			/* R */
			Chip_TIMER_SetMatch(RMOTOR_TIMER, PWM_LMOTOR_PIN_MATCH_NUMBER, MOTORS_PERIOD_RESET - power);
		}
	}
}

void Motors_Backward(uint16_t power)
{
	if (motors_on) {
		/* Change direction of the motors if it's not forward */
		if (!Motors_areBackward()) {
			Motors_changeDir(BACKWARD);
		}
		/* change PWM output */
		if ((0 <= power) && (power <= MOTORS_PERIOD_RESET )) {
			/* L */
			Chip_TIMER_SetMatch(LMOTOR_TIMER, PWM_LMOTOR_PIN_MATCH_NUMBER, MOTORS_PERIOD_RESET - power);

			/* R */
			Chip_TIMER_SetMatch(RMOTOR_TIMER, PWM_LMOTOR_PIN_MATCH_NUMBER, MOTORS_PERIOD_RESET - power);
		}
	}
}

void Motors_Start()
{

	/* Enable PWM */

		/* L */
	LMOTOR_TIMER -> PWMC = (1 << PWM_LMOTOR_PIN_MATCH_NUMBER);

		/* R */
	RMOTOR_TIMER -> PWMC = (1 << PWM_RMOTOR_PIN_MATCH_NUMBER);

	/* Enable TIMERs */

		/* L */
	Chip_TIMER_Enable(LMOTOR_TIMER);

		/* R */
	Chip_TIMER_Enable(RMOTOR_TIMER);

	motors_on = 1;

	/* Move forward with 0 power */
	Motors_changeDir(FORWARD);
	Motors_Forward(0);
}

/* Deal with low value issues, for example if the wheels begin to turn after 100 = 5 V * (100/255), you might want them to turn after 50, to do that set minValue to 100 and minRunValue = 50 */
static void deal_with_low_values(int *motorPower, int minValue, int minRunValue)
{
	if (*motorPower >= 0) {
		if ((*motorPower >= minRunValue) && (*motorPower <= minValue)) {
			*motorPower = minValue;
		}
	} else {
		if ((*motorPower <= -minRunValue) && (*motorPower >= -minValue)) {
			*motorPower = -minValue;
		}
	}
}


void Motors_Move(float powerFloat)
{
	if (powerFloat < -100) {
		powerFloat = powerFloat - 300;
	} else if (powerFloat > 100) {
		powerFloat = powerFloat + 300;
	} else {
		powerFloat = 0;
	}

	int motorPowerFloor = floor(powerFloat);
	motorPowerFloor = (motorPowerFloor <= -MOTORS_PERIOD_RESET) ? -MOTORS_PERIOD_RESET : ((motorPowerFloor >= MOTORS_PERIOD_RESET) ? MOTORS_PERIOD_RESET : motorPowerFloor);			//constrain the range
	//deal_with_low_values(&motorPowerFloor, 400, 200);

	if (motorPowerFloor <= 0){
		Motors_Forward(abs(motorPowerFloor));
	} else {
		Motors_Backward(abs(motorPowerFloor));
	}
}
