#ifndef UART_H_
#define UART_H_

#ifndef CRYPTO_H_
typedef unsigned char byte;
#endif

#include <stdint.h>
#define BAUDRATE 9600 //Baudrate
#define TIMEOUT_ACK 10

void Init_UART();													// Init UART
void Quit_UART();													// Quit UART
void UART_PutHEX(uint32_t hex);										// Send a hex value
void UART_PutCHAR(char c);											// Send a char value
void UART_PutSTR(char *str);										// Send a string value
void UART_PutINT(uint16_t n);										// Send a uint32t value
void UART_PutFLOAT(float f, int precision);							// Send a float value
uint8_t UART_Read_max_nB(uint32_t n);								// Read UART, stop when fifo empty or when n bytes have been read
void UART_Read_PID();												// Read UART, waiting for the good sentence to modify PID
void UART_Test_SHA256();											// Verify that SHA256 is working
void UART_Read_Replay();											// Send what we receive
void UART_Test_HMAC_SHA256();										// Verify that HMAC-SHA256 is working
void UART_Read(uint8_t* b);											// Block till we read 14 byte
uint8_t UART_Handshake(const byte KEY[32], byte DERIVATE_KEY[32]);	// Handshake protocol, return 1 if failed, 0 on success
uint8_t UART_SendSTR_Signed(const byte *message, const uint16_t sizeMessage, const byte KEY[32], uint8_t is_ACK);
uint16_t UART_ReceiveSTR_Signed(byte MESSAGE[], const byte KEY[32]);		//return the bytes read before error or overflow

#endif /* UART_H_ */
