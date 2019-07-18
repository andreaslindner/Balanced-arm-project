#include <lMotor.h>
#include <chip.h>

volatile uint8_t lMotor_on = 0;

void Init_lMotor()
{
	/* Change direction of pins 1_1 and 1_2 */
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 1);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 1, 2);

	/* Change pin function */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_1, IOCON_FUNC1);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_2, IOCON_FUNC1);

	/* Init TIMER used to produce PWM */
	Chip_TIMER_Init(LPC_TIMER32_1);
	Chip_TIMER_PrescaleSet(LPC_TIMER32_1, Chip_Clock_GetSystemClockRate() / (PERIOD_RESET_LMOTOR * LFREQ_LOOP));	//set prescale to have PERIOD_RESET_LMOTOR TICKs of the TIMER <-> 1 ms
	Chip_TIMER_Disable(LPC_TIMER32_1);
	Chip_TIMER_Reset(LPC_TIMER32_1);

	/* Set the reset match of the TIMER */
	Chip_TIMER_SetMatch(LPC_TIMER32_1, 2, PERIOD_RESET_LMOTOR);
	Chip_TIMER_ResetOnMatchDisable(LPC_TIMER32_1, 0);
	Chip_TIMER_ResetOnMatchDisable(LPC_TIMER32_1, 1);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER32_1, 2);

	/* Turn off the motor, block the wheel */
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 1, true);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 2, true);
	lMotor_on = 0;
}

void lMotor_Start()
{
	lMotor_on = 1;
	Chip_TIMER_Enable(LPC_TIMER32_1);
	lMotor_changeDir(FORWARD);
	lMotor_Forward(0);
}

void lMotor_Stop()
{
	Chip_TIMER_Disable(LPC_TIMER32_1);

	/* Turn of the motor, block the wheel */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_1, IOCON_FUNC1);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_2, IOCON_FUNC1);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 1, true);
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 2, true);
	lMotor_on = 0;
}

void lMotor_changeDir(uint8_t direction)
{
	if (lMotor_on) {
		if (direction == FORWARD) {
			LPC_TIMER32_1 -> PWMC = 0x01;
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_1, IOCON_FUNC3);
			Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_2, IOCON_FUNC1);

			/* Free wheel */
			Chip_TIMER_SetMatch(LPC_TIMER32_1, 0, PERIOD_RESET_LMOTOR); // set power to 0%
			Chip_TIMER_Reset(LPC_TIMER32_1);
			Chip_GPIO_SetPinState(LPC_GPIO, 1, 2, false);

		} else {
			if (direction == BACKWARD) {
				LPC_TIMER32_1 -> PWMC = 0x02;
				Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_2, IOCON_FUNC3);
				Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_1, IOCON_FUNC1);

				/* Free wheel */
				Chip_TIMER_SetMatch(LPC_TIMER32_1, 1, PERIOD_RESET_LMOTOR); // set power to 0%
				Chip_TIMER_Reset(LPC_TIMER32_1);
				Chip_GPIO_SetPinState(LPC_GPIO, 1, 1, false);
			}

		}
	}
}

uint8_t lMotor_isForward()
{
	return((LPC_TIMER32_1 -> PWMC) & (0x01));
}

void lMotor_Backward(uint8_t perPower)
{
	if (lMotor_on) {
		if (lMotor_isForward()) {
			lMotor_changeDir(BACKWARD);
		}
		if ((0 <= perPower) && (perPower <= PERIOD_RESET_LMOTOR )) {
			Chip_TIMER_SetMatch(LPC_TIMER32_1, 1, PERIOD_RESET_LMOTOR - perPower);
			Chip_TIMER_Reset(LPC_TIMER32_1);
		}
	}
}

void lMotor_Forward(uint8_t perPower)
{
	if (lMotor_on) {
		if (lMotor_isForward() == 0) {
			lMotor_changeDir(FORWARD);
		}
		if ((0 <= perPower) && (perPower <= PERIOD_RESET_LMOTOR )) {
			Chip_TIMER_SetMatch(LPC_TIMER32_1, 0, PERIOD_RESET_LMOTOR - perPower);
			Chip_TIMER_Reset(LPC_TIMER32_1);
		}
	}
}

