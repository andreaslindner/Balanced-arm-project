#ifndef LED_H_
#define LED_H_

#define PERIOD_RESET_LED 100 // Number of tick ( = 0.01 ms ) for pwm period
#include <stdint.h>

void Init_LED();																//Init LED pins (dir, value, function)
void Init_TIMER_LED(uint32_t periodReset);										//Init the timer for pwm green LED and blue LED
void Set_TIMER_Match(uint8_t nummat, uint8_t dutyPer, uint32_t periodReset);	//Change pwm duty cycle
void Quit_TIMER_LED();															//Quit TIMER LED

#endif
