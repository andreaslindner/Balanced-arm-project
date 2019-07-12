/*
 * timer.h
 *
 *  Created on: 2 juil. 2019
 *      Author: bcoste
 */
#include <stdint.h>
#ifndef TIMER_H_
#define TIMER_H_

uint32_t TIMER_Get_Counter();			// Return the current timer count and reset count
void Init_TIMER();						// Init TIMER 32_0
void TIMER_Enable();					// Start couting
void TIMER_Start();

#endif /* TIMER_H_ */
