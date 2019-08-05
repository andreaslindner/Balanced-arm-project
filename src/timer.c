#include <chip.h>
#include <timer.h>


void Init_TIMER()
{
	/* Init TIMER*/
	Chip_TIMER_Init(LPC_TIMER16_1);
	Chip_TIMER_PrescaleSet(LPC_TIMER16_1, Chip_Clock_GetSystemClockRate()/PRESCALE);
	Chip_TIMER_Disable(LPC_TIMER16_1);
}

void TIMER_Reset()
{
	Chip_TIMER_Reset(LPC_TIMER16_1);
}

void TIMER_Start()
{
	Chip_TIMER_Reset(LPC_TIMER16_1);
	Chip_TIMER_Enable(LPC_TIMER16_1);
}

void TIMER_Wait_Till(uint16_t checkpoint)
{
	while ((LPC_TIMER16_1 -> TC) < checkpoint);
	return;
}

uint16_t TIMER_getCounter()
{
	uint16_t a = LPC_TIMER16_1 -> TC;

	Chip_TIMER_Reset(LPC_TIMER16_1);
	return a;
}

void TIMER_Stop()
{
	Chip_TIMER_Disable(LPC_TIMER16_1);
	Chip_TIMER_Reset(LPC_TIMER16_1);
}

void TIMER_DeInit()
{
	Chip_TIMER_DeInit(LPC_TIMER16_1);
}
