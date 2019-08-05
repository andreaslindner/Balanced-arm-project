#ifndef UART_H_
#define UART_H_

#ifndef CRYPTO_H_
typedef unsigned char byte;
#endif

#include <stdint.h>
#define BAUDRATE 9600 //Baudrate
#define TIMEOUT_ACK 10

uint8_t UART_Read_max_nB(uint32_t n);								// Read UART, stop when fifo empty or when n bytes have been read
void UART_Test_SHA256();											// Verify that SHA256 is working
void UART_Test_HMAC_SHA256();										// Verify that HMAC-SHA256 is working


/*	---CONFIG UART--- */

/**
 * @brief	Configure UART
 * @param	Nothing
 * @return	Nothing
 * @note	To change the baud rate, modify BAUDRATE uart.h
 */
void Init_UART();


/* ---UART SEND FUNCTIONS--- */

/**
 * @brief	Transform a unsigned int into hex string and send it through UART
 * @param	hex :  unsigned int you want to send
 * @return	Nothing
 * @note	Write is blocking (wait till the data is in the FIFO)
 */
void UART_PutHEX(uint32_t hex);

/**
 * @brief	Send a char through UART
 * @param	c : char you want to send
 * @return	Nothing
 * @note	Write is blocking (wait till the data is in the FIFO)
 */
void UART_PutCHAR(char c);

/**
 * @brief	Send a string through UART
 * @param	str : address of the string you want to send
 * @return	Nothing
 * @note	Write is blocking (wait till the data is in the FIFO)
 */
void UART_PutSTR(char *str);

/**
 * @brief	Transform an unsigned int into int string and send it
 * @param	n : unsigned int you want to send as int
 * @return	Nothing
 * @note	Write is blocking (wait till the data is in the FIFO)
 */
void UART_PutINT(uint16_t n);

/**
 * @brief	Transform an float into string and send it
 * @param	f : float you want to send as int
 * 			precision : number of digits
 * @return	Nothing
 * @note	Write is blocking (wait till the data is in the FIFO)
 */
void UART_PutFLOAT(float f, int precision);

/* ---UART READ FUNCTIONS--- */

/**
 * @brief	Read one byte from UART
 * @param	b : address where you want to store the byte
 * @return	Nothing
 * @note	Read is blocking
 */
void UART_Read(uint8_t* b);

/**
 * @brief	Analyze the bytes received and modify the PID parameters if the protocol is respected
 * @param	Nothing
 * @return	Nothing
 * @note	Example : send "p+=0.5P" to set kp to 0.5, send "p+0.5P" to increase kp by 0.5. You can use "d+20D" to increase kd and "i+20I" to increase ki
 */
void UART_Read_PID();

/**
 * @brief	Send data we receive
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void UART_Read_Replay();

/* ---UART CRYPTO FUNCTIONS--- */

/**
 * @brief	Wait for handshake
 * @param	KEY : 32-bit buffer where the crypto 32-bit key is stored
 * 			DERIVATE_KEY : 32-bit buffer where the derivate key deduced from handshake will be stored
 * @return	return 0 if handshake successful, 1 if handshake failed
 * @note	Nothing
 */
uint8_t UART_Handshake(const byte KEY[32], byte DERIVATE_KEY[32]);

/**
 * @brief	Send a message through UART respecting the crypto protocol
 * @param	message : address of the message
 * 			sizeMessage : size of the message
 * 			KEY : 32-bit key used to encrypt
 * 			is_ACK : must be set to 1 if the message sent is ACK and set to 0 if not.
 * @return	return 0 if it is successful, 1 if not.
 * @note	You can't send more than 14 bytes in a row for the moment, so sizeMessage should be lower or equal to 14
 */
uint8_t UART_SendSTR_Signed(const byte *message, const uint16_t sizeMessage, const byte KEY[32], uint8_t is_ACK);

/**
 * @brief	Receive a message and encrypt it
 * @param	MESSAGE : address of the message
 * 			KEY : 32-bit key used to decrypt
 * @return	return 14 if it is successful, 0 if not.
 * @note	Nothing
 */
uint16_t UART_ReceiveSTR_Signed(byte MESSAGE[], const byte KEY[32]);

#endif /* UART_H_ */
