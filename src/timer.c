#include <timer.h>
#include <chip.h>

void Init_TIMER()
{
	Chip_TIMER_Init(LPC_TIMER32_0);
	Chip_TIMER_PrescaleSet(LPC_TIMER32_0, Chip_Clock_GetSystemClockRate()/10000);	//set prescale to have 1 TICK <-> 100 µs
	Chip_TIMER_Disable(LPC_TIMER32_0);
	Chip_TIMER_Reset(LPC_TIMER32_0);
}

uint32_t TIMER_Get_Counter()
{
	uint32_t res = Chip_TIMER_ReadCount(LPC_TIMER32_0);
	Chip_TIMER_Reset(LPC_TIMER32_0);
	return res;
}

void TIMER_Enable()
{
	Chip_TIMER_Enable(LPC_TIMER32_0);
}
