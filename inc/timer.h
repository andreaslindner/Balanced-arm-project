#ifndef TIMER_H_
#define TIMER_H_

#define PRESCALE 10000 // 0.1 ms <-> 1 tick

/* ---CONFIGURE TIMER--- */

/**
 * @brief	Init 16-bit TIMER 1
 * @param	Nothing
 * @return	Nothing
 * @note	Change RESCALE defined above to change the period of one tick of the counter, for example for PRESCALE = 10 000, we got period = 1 / 10 000 = 0.1 ms of one tick of the counter
 */
void Init_TIMER();

/**
 * @brief	De init the 16-bit TIMER 1
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void TIMER_DeInit();


/* ---TIMER MANAGEMENT--- */

/**
 * @brief	Start the counter
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void TIMER_Start();

/**
 * @brief	Reset the value of the counter
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void TIMER_Reset();

/**
 * @brief	Stop the counter
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void TIMER_Stop();

/**
 * @brief	Get the current value of the counter and reset the timer
 * @param	Nothing
 * @return	current value of the counter
 * @note	Nothing
 */
uint16_t TIMER_getCounter();

/**
 * @brief	Block till the counter value is equal or higher than the checkpoint
 * @param	checkpoint : match value
 * @return	Nothing
 * @note	Nothing
 */
void TIMER_Wait_Till(uint16_t checkpoint);
#endif /* TIMER_H_ */
