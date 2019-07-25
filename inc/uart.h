#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#define BAUDRATE 9600 //Baudrate
#define SINGLE_THREADED
#define NO_FILESYSTEM
#undef NO_SHA256

void Init_UART();							// Init UART
void Quit_UART();							// Quit UART
void UART_PutHEX(uint32_t hex);				// Send a hex value
void UART_PutCHAR(char c);					// Send a char value
void UART_PutSTR(char *str);				// Send a string value
void UART_PutINT(uint16_t n);				// Send a uint32t value
void UART_PutFLOAT(float f, int precision);	// Send a float value
uint8_t UART_Read_max_nB(uint32_t n);		// Read UART, stop when fifo empty or when n bytes have been read
void UART_Read_PID();						// Read UART, waiting for the good sentence to modify PID
void UART_Test_SHA256();					// Verify that SHA256 is working
void UART_Read_Replay();					// Send what we receive

#endif /* UART_H_ */
