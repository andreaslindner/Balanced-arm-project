#include <rMotor.h>
#include <chip.h>

volatile uint8_t rMotor_on = 0;

void Init_rMotor()
{
	/* Change direction of pins 1_1 and 1_2 */
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 1);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 2);

	/* Change pin function */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_9, IOCON_FUNC0);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_8, IOCON_FUNC0);

	/* Init TIMER used to produce PWM */
	Chip_TIMER_Init(LPC_TIMER16_0);
	Chip_TIMER_PrescaleSet(LPC_TIMER16_0, Chip_Clock_GetSystemClockRate()/1000);	//set prescale to have 1 TICK <-> 100 µs = 0.1 ms
	Chip_TIMER_Disable(LPC_TIMER16_0);
	Chip_TIMER_Reset(LPC_TIMER16_0);

	/* Set the reset match of the TIMER */
	Chip_TIMER_SetMatch(LPC_TIMER16_0, 2, PERIOD_RESET_RMOTOR);
	Chip_TIMER_ResetOnMatchDisable(LPC_TIMER16_0, 0);
	Chip_TIMER_ResetOnMatchDisable(LPC_TIMER16_0, 1);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER16_0, 2);

	/* Turn off the motor, block the wheel */
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 9, true);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 8, true);
	rMotor_on = 0;
}

void rMotor_Start()
{
	rMotor_on = 1;
	Chip_TIMER_Enable(LPC_TIMER16_0);
	rMotor_changeDir(FORWARD);
	rMotor_Forward(0);
}

void rMotor_Stop()
{
	Chip_TIMER_Disable(LPC_TIMER16_0);

	/* Turn of the motor, block the wheel */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_9, IOCON_FUNC0);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_8, IOCON_FUNC0);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 9, true);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 8, true);
	rMotor_on = 0;
}

void rMotor_changeDir(uint8_t direction)
{
	if (rMotor_on) {
		if (direction == FORWARD) {
			LPC_TIMER16_0 -> PWMC = 0x02;
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_9, IOCON_FUNC2);
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_8, IOCON_FUNC0);

			/* Free wheel */
			Chip_TIMER_SetMatch(LPC_TIMER16_0, 1, PERIOD_RESET_RMOTOR); // set power to 0%
			Chip_GPIO_SetPinState(LPC_GPIO, 0, 8, false);

		} else {
			if (direction == BACKWARD) {
				LPC_TIMER16_0 -> PWMC = 0x01;
				Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_8, IOCON_FUNC2);
				Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_9, IOCON_FUNC0);

				/* Free wheel */
				Chip_TIMER_SetMatch(LPC_TIMER16_0, 0, PERIOD_RESET_RMOTOR); // set power to 0%
				Chip_GPIO_SetPinState(LPC_GPIO, 0, 9, false);
			}

		}
	}
}

uint8_t rMotor_isForward()
{
	return(((LPC_TIMER16_0 -> PWMC) & (0x02)) == 2);
}

void rMotor_Backward(uint8_t perPower)
{
	if (rMotor_on) {
		if (rMotor_isForward()) {
			rMotor_changeDir(BACKWARD);
		}
		if ((0 <= perPower) && (perPower <= PERIOD_RESET_RMOTOR )) {
			Chip_TIMER_SetMatch(LPC_TIMER16_0, 0, (100 - perPower) * PERIOD_RESET_RMOTOR / 100);
		}
	}
}

void rMotor_Forward(uint8_t perPower)
{
	if (rMotor_on) {
		if (rMotor_isForward() == 0) {
			rMotor_changeDir(FORWARD);
		}
		if ((0 <= perPower) && (perPower <= PERIOD_RESET_RMOTOR )) {
			Chip_TIMER_SetMatch(LPC_TIMER16_0, 1, (100 - perPower) * PERIOD_RESET_RMOTOR / 100);
		}
	}
}

