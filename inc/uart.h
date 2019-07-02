#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#define BAUDRATE 115200 //Baudrate

void Init_UART();							// Init UART
void Quit_UART();							// Quit UART
void UART_PutHEX(uint32_t hex);				// Send a hex value
void UART_PutCHAR(char c);					// Send a char value
void UART_PutSTR(char *str);				// Send a string value
void UART_PutINT(uint16_t n);				// Send a uint32t value
uint8_t UART_Read_max_nB(uint32_t n);		// Read UART, stop when fifo empty or when n bytes have been read

#endif /* UART_H_ */
