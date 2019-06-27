#include <chip.h>
#include <led.h>

void Init_LED()
{
	/* Reset func of pins 7,8,9 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_7, (IOCON_FUNC0));
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_8, (IOCON_FUNC0));
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_9, (IOCON_FUNC0));

	/* Set the PIO0_7,8,9 as output */
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 7);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 8);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO, 0, 9);

	/* Set true to all of these three pin's states */
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 7, true);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 8, true);
	Chip_GPIO_SetPinState(LPC_GPIO, 0, 9, true);
}

void Init_TIMER_LED(uint32_t periodReset)
{
	Chip_TIMER_Init(LPC_TIMER16_0);
	Chip_TIMER_PrescaleSet(LPC_TIMER16_0, Chip_Clock_GetSystemClockRate()/10000);	//set prescale to have 1 TICK <-> 100 µs = 0.1 ms
	Chip_TIMER_Disable(LPC_TIMER16_0);
	Chip_TIMER_Reset(LPC_TIMER16_0);
	LPC_TIMER16_0 -> PWMC = 0x03; // enable pwm mode on match 0 and 1 ( linked to LED G and B)
	Chip_TIMER_SetMatch(LPC_TIMER16_0, 2, periodReset);
	Chip_TIMER_ResetOnMatchDisable(LPC_TIMER16_0, 0);
	Chip_TIMER_ResetOnMatchDisable(LPC_TIMER16_0, 1);
	Chip_TIMER_ResetOnMatchEnable(LPC_TIMER16_0, 2); // match 2 for reset every periodReset 0.1 ms
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_8, IOCON_FUNC2);
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_9, IOCON_FUNC2);
}

void Set_TIMER_Match(uint8_t nummat, uint8_t dutyPer, uint32_t periodReset){
	Chip_TIMER_SetMatch(LPC_TIMER16_0, nummat, (dutyPer * periodReset) / 100);
}

void Quit_TIMER_LED()
{
	Chip_TIMER_DeInit(LPC_TIMER16_0);
}
