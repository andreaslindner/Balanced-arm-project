#ifndef TIMER_H_
#define TIMER_H_

#define PRESCALE 10000 // 0.1 ms <-> 1 tick

void Init_TIMER();										//Init TIMER
void TIMER_Reset();										//Reset TIMER
void TIMER_Start();										//Start TIMER
void TIMER_Wait_Till(uint16_t checkpoint);				//While counter is under checkpoint, block
uint16_t TIMER_getCounter();							//Return current value of the TIMER
void TIMER_Stop();										//Stop the TIMER
void TIMER_DeInit();									//DeInit the TIMER

#endif /* TIMER_H_ */
