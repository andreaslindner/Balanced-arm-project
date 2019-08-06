#include <chip.h>
#include <uart.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <crypto.h>
#include <timer.h>

/* UART_BUFFER and its counter, these variables are used in UART_Read_PID() */

uint8_t UART_BUFFER_counter = 0;
uint8_t UART_BUFFER[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/* Necessary if we are tuning the PID */

extern volatile float kp;
extern volatile float ki;
extern volatile float kd;
extern volatile float errorSum;
extern volatile float alpha;

/* Necessary if we are using crypto */

extern uint16_t nounce;
const byte ACK[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};


/*	---CONFIG UART--- */

void Init_UART()
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, (IOCON_FUNC1 | IOCON_MODE_INACT));	// RXD
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, (IOCON_FUNC1 | IOCON_MODE_INACT));	// TXD
	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, BAUDRATE);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_USART, (1) | (1<<1) | (1<<2));
	Chip_UART_TXEnable(LPC_USART);

	/* Disable all interrupts */
	Chip_UART_IntDisable(LPC_USART, ~0);
}

/* ---UART SEND FUNCTIONS--- */

void UART_PutCHAR(char c)
{
	Chip_UART_SendBlocking(LPC_USART, &c, 1);
}

void UART_PutSTR(char *str)
{
	while (*str != '\0') {
		UART_PutCHAR(*str++);
	}
}

void UART_PutSTR2(char *str, uint16_t size)
{
	uint16_t counter = 0;
	while (counter < size) {
		UART_PutCHAR(*str++);
		counter++;
	}
}

void UART_PutHEX(uint32_t hex)
{
	unsigned int rb;
	char rc;

	UART_PutSTR("0x");
	rb=32;
	while((rb))
	{
		rb-=4;
		rc=(hex>>rb)&0xF;
		if(rc>9) rc+=(-10 + 'A'); else rc+='0';
		UART_PutCHAR(rc);
	}
}

void UART_PutINT(uint16_t n)
{
	char str[10] = "";
	int val = (int16_t) n;
	itoa(val,str,10);
	UART_PutSTR(str);
}


void UART_PutFLOAT(float f, int precision) {
	char str[30];
	int32_t a,b,c,k,l=0,m,i=0;

	// check for negetive float
	if(f<0.0)
	{

		str[i++]='-';
		f*=-1;
	}

	a=f;	// extracting whole number
	f-=a;	// extracting decimal part
	k = precision;

	// number of digits in whole number
	while(k>-1)
	{
		l = pow(10,k);
		m = a/l;
		if(m>0)
		{
			break;
		}
	k--;
	}

	// number of digits in whole number are k+1

	/*
	extracting most significant digit i.e. right most digit , and concatenating to string
	obtained as quotient by dividing number by 10^k where k = (number of digit -1)
	*/

	for(l=k+1;l>0;l--)
	{
		b = pow(10,l-1);
		c = a/b;
		str[i++]=c+48;
		a%=b;
	}
	str[i++] = '.';

	/* extracting decimal digits till precision */

	for(l=0;l<precision;l++)
	{
		f*=10.0;
		b = f;
		str[i++]=b+48;
		f-=b;
	}

	str[i]='\0';
	UART_PutSTR(str);
}

/* ---UART READ FUNCTIONS--- */

void UART_Read(uint8_t* b) {
	Chip_UART_ReadBlocking(LPC_USART, b, 1);
}

void UART_Read_Replay()
{
	char byte;
	while (Chip_UART_Read(LPC_USART, &byte, 1) == 0);
	UART_PutCHAR(byte);
}

/* clean buffer UART_BUFFER */
static void cleanBuffer()
{
	uint8_t i = 0;
	for (; i < 20; ++i) {
		UART_BUFFER[i] = 0;
	}
	UART_BUFFER_counter = 0;
	UART_PutSTR("Buffer cleaned\r\n\0");
}

/* put to UART kp, kd, ki */
static void UART_PutPID()
{
	UART_PutSTR(" -> Kp : \0");
	UART_PutFLOAT(kp, 6);
	UART_PutSTR(", ");
	UART_PutSTR("Kd : \0");
	UART_PutFLOAT(kd, 6);
	UART_PutSTR(", ");
	UART_PutSTR("Ki : \0");
	UART_PutFLOAT(ki, 6);
	UART_PutSTR("\r\n0");
}

/* analyze the buffer UART_BUFEFR to check if it follows the protocol of UART_Read_PID() */
static void analyseBuffer()
{
	float sign = 1;
	uint8_t equal = 0;
	float sum = 0, sum2 = 0;
	uint8_t counter_number = 0, counter_number2 = 0;

	if (UART_BUFFER_counter < 3) {
		cleanBuffer();
		return;
	} else {
		if (
				(UART_BUFFER[0] == 112)
			||	(UART_BUFFER[0] == 105)
			|| 	(UART_BUFFER[0] == 100)
			||	(UART_BUFFER[0] == 97)
			) {																						//if first byte a p, i or d or a
			if (UART_BUFFER[1] == 61) {																	//if we've got an equal
					equal = 1;
			}
			if (UART_BUFFER[1 + equal] == 43) {																	//if second byte is +
				sign = 1;
			} else {																					//else
				if (UART_BUFFER[1 + equal] == 45) {															//if second bytes is -
					sign = -1;
				} else {																					//if second byte is not + or -
					cleanBuffer();
					return;
				}
			}

			while 	(																					//while it's a number and we don't overflow
						(counter_number < UART_BUFFER_counter)
					&& 	(48 <= UART_BUFFER[2 + equal + counter_number])
					&& 	(UART_BUFFER[2 + equal + counter_number] <= 57)
					) {
				sum *= 10;
				sum += UART_BUFFER[2 + equal + counter_number] - 48;
				++counter_number;
			}

			if (counter_number == UART_BUFFER_counter) {													//if we overflow
				cleanBuffer();
				return;
			}

			if (UART_BUFFER[2 + equal + counter_number] == 46) {													//if its a dot
				counter_number++;
				while 	(																					//while it's a number and we don't overflow
							((2 + equal + counter_number + counter_number2) < UART_BUFFER_counter)
						&& 	(48 <= UART_BUFFER[2 + equal + (counter_number + counter_number2)])
						&& 	(UART_BUFFER[2 + equal + (counter_number + counter_number2)] <= 57)
						) {
					sum2 *= 10;
					sum2 += UART_BUFFER[2 + equal + (counter_number + counter_number2)] - 48;
					++counter_number2;
				}
				for (int i = 0; i < counter_number2; ++i) {
					sum2 /= 10;
				}

				if (counter_number == UART_BUFFER_counter) {													//if we overflow
					cleanBuffer();
					return;
				}
			}

			if ((UART_BUFFER[2 + equal + (counter_number + counter_number2)] + 32) == UART_BUFFER[0]) {	//if the buffer is close correctly
				if (equal == 1) {																			//if we had an equal
					switch (UART_BUFFER[0]) {																	//if its p,i,d or a

						case 112 : 	kp = (sign) * (sum + sum2);														//p
									errorSum = 0;
									UART_PutPID();
									break;

						case 105: 	ki = (sign) * (sum + sum2);														//i
									errorSum = 0;
									UART_PutPID();
									break;

						case 100 : 	kd = (sign) * (sum + sum2);														//d
									errorSum = 0;
									UART_PutPID();
									break;

						case 97  : 	alpha = (sign) * (sum + sum2);													//a
									errorSum = 0;
									UART_PutSTR(" -> alpha = ");
									UART_PutFLOAT(alpha, 6);
									UART_PutSTR("\r\n");
									break;

						default  : 	break;																		//default
					}
					cleanBuffer();
					return;
				} else {																					//else
					switch (UART_BUFFER[0]) {																	//if its p,i,d or a

						case 112 : 	kp += (sign) * (sum + sum2);													//p
									errorSum = 0;
									UART_PutPID();
									break;

						case 105: 	ki += (sign) * (sum + sum2);													//i
									errorSum = 0;
									UART_PutPID();
									break;

						case 100 : 	kd += (sign) * (sum + sum2);													//d
									errorSum = 0;
									UART_PutPID();
									break;

						case 97  : 	alpha += (sign) * (sum + sum2);													//a
									errorSum = 0;
									UART_PutSTR(" -> alpha = ");
									UART_PutFLOAT(alpha, 6);
									UART_PutSTR("\r\n");
									break;

						default  : 	break;																			//default
					}
					cleanBuffer();
					return;
				}
			} else {																					//else
				cleanBuffer();
				return;
			}

		} else {																					//else
			cleanBuffer();
			return;
		}
	}
}

void UART_Read_PID()
{
	uint8_t byte;
	UART_Read(&byte);
	if (byte == 10) {
		return;
	}
	UART_BUFFER[UART_BUFFER_counter] = byte;
	UART_BUFFER_counter++;

	if ((byte == 80) || (byte == 73) || (byte == 68) || (byte == 65)) {				//byte is a P, I or D or A (end of the protocol we analyse the buffer)
		analyseBuffer();
	}

	if (UART_BUFFER_counter == 20) {
		cleanBuffer();
	}
}

/* ---UART CRYPTO FUNCTIONS--- */

uint8_t UART_Handshake(const byte KEY[32], byte *DERIVATE_KEY)
{
	uint8_t same_hash = 1;
	volatile int i = 0;
	byte byt, IV[16], MESSAGE[32], CIPHER_HASH[32], PLAIN_HASH[32], MY_HASH[32];


	//We get IV(16B)|M(32B)|H(32B)
	for (i = 0; i < 80; ++i) {
		if (i < 16) {
			if (i == 0) {
				while(Chip_UART_Read(LPC_USART, &IV[i], 1) == 0);
				//begin timeout packet measurement
				Init_TIMER();
				TIMER_Start();
			} else {
				while((TIMER_getCounter() < TIMEOUT_PACKET * PRESCALE) && (Chip_UART_Read(LPC_USART, &IV[i], 1) == 0));
			}
		} else {
			if (i < 48){
				while((TIMER_getCounter() < TIMEOUT_PACKET * PRESCALE) && (Chip_UART_Read(LPC_USART, &MESSAGE[i - 16], 1) == 0));
			} else {
				while((TIMER_getCounter() < TIMEOUT_PACKET * PRESCALE) && (Chip_UART_Read(LPC_USART, &CIPHER_HASH[i - 48], 1) == 0));
			}
		}
		if ((TIMER_getCounter() > TIMEOUT_PACKET * PRESCALE) && (i != 79)) {
			i = -1;
			TIMER_Stop();
			TIMER_DeInit();
		}
	}
	TIMER_Stop();
	TIMER_DeInit();

	// We decrypt the hash using AES_256_CBC

	Decrypt_AES258_CBC(KEY, PLAIN_HASH, CIPHER_HASH, IV);

	// We verify that we've got the same hash

	HASH_SHA256(MESSAGE, 32, MY_HASH);

	for (i = 0; i < 32; ++i) {
		if (PLAIN_HASH[i] != MY_HASH[i]) {
			same_hash = 0;
			break;
		}
	}

	if (!same_hash) {
		return 1;
	}

	// Add 1 to IV to change vector
	i = 0;

	while ((i != 16) && (IV[i] == 255)) {
		IV[i] = 0;
		++i;
	}
	if (i != 16) {
		IV[i] += 1;
	}


	//Compute the derivate key
	Encrypt_AES258_CBC(KEY, DERIVATE_KEY, MESSAGE, IV);

	/* Send ACK */
	UART_SendSTR_Signed(ACK, 14, DERIVATE_KEY, 1);

	/*increment nounce */
	nounce++;


	return 0;
}

/* Send multiple bytes through UART */
static void UART_Send_nB(const byte *message, const uint8_t sizeMessage) {
	Chip_UART_SendBlocking(LPC_USART, message, (int)sizeMessage);
}

/* check if one is equal to two on the size first bytes */
static uint8_t ByteArray_Equal(const byte one[], const byte two[], uint8_t size)
{
	for (uint8_t i = 0; i < size; ++i) {
		if (one[i] != two[i]) {
			return 1;
		}
	}
	return 0;
}

/* Wait for ACK, KEY is the 32-bit key used to encrypt data */
static uint8_t Wait_ACK(const byte KEY[32])
{
	uint8_t counter = 0;
	uint8_t timeout = 1;
	byte PLAIN_TEXT[16];
	byte HASH[32];
	byte HASH_VERIFY[32];
	uint16_t received_nounce;

	Init_TIMER();
	TIMER_Start();

	/* while we don't timeout, we try to read ACK */
	while (TIMER_getCounter() < (TIMEOUT_ACK * PRESCALE)) {

		if (counter < 16) {
			if (Chip_UART_Read(LPC_USART, &PLAIN_TEXT[counter], 1) > 0) {
				counter++;
			}
		} else {
			if (Chip_UART_Read(LPC_USART, &HASH[counter - 16], 1) > 0) {
				counter++;
				if (counter == 48) {
					timeout = 0;
					break;
				}
			}
		}
	}

	TIMER_Stop();
	TIMER_DeInit();

	if (timeout == 1) {
		return 1;
	}

	/* we didn't timeout, we have to verify the message now */
	if (ByteArray_Equal(PLAIN_TEXT, ACK, 14) != 0) {
		return 2;
	}

	/*ACK is ok, verify nouce now */
	received_nounce = (PLAIN_TEXT[15] << 8) | (PLAIN_TEXT[14]);
	if (received_nounce != nounce) {
		return 3;
	}

	/* Now we verify the HASH */
	HMAC_SHA256(PLAIN_TEXT, 16, KEY, 32, HASH_VERIFY);
	if (ByteArray_Equal(HASH, HASH_VERIFY, 32) != 0) {
		return 4;
	}

	return 0;
}

uint8_t UART_SendSTR_Signed(const byte message[], const uint16_t sizeMessage, const byte KEY[32], uint8_t is_ACK)
{
	byte HASH[32];
	byte MESSAGE_S[16];
	uint16_t counter = 0;
	uint8_t i;

	/* for each block of 14 bytes */
	for (; counter < sizeMessage; counter+= 14) {

		/* fill with the message or '\0' if empty */
		for (i = 0; i < 14; ++i) {
			if ((counter + i) < sizeMessage) {
				MESSAGE_S[i] = message[counter + i];
			} else {
				MESSAGE_S[i] = 0;
			}
		}

		/* 2 last bytes are nounce */
		MESSAGE_S[14] = (byte) (nounce & 0xFF);				//first byte of nounce
		MESSAGE_S[15] = (byte) ((nounce & 0xFF00) >> 8);	//second byte of nounce

		/* get the HMAC */
		HMAC_SHA256(MESSAGE_S, 16, KEY, 32, HASH);

		/* Send message|nounce|hash = 48 bytes */
		UART_Send_nB(MESSAGE_S, 16);
		UART_Send_nB(HASH, 32);

		/* Wait for ACK if its not ACK */
		if (!is_ACK) {
			if (Wait_ACK(KEY) != 0) {
				return 1;
			}
			nounce++;
		}
	}
	return 0;
}

uint16_t UART_ReceiveSTR_Signed(byte MESSAGE[], const byte KEY[32])
{
	byte HASH[32];
	byte PLAIN_TEXT[16];
	volatile int i;
	byte HASH_VERIFY[32];
	uint16_t received_nounce;

	/* receive one packet of 48 bytes */
	for (i = 0; i < 48; ++i) {
		if (i < 16) {
			if (i == 0) {
				while(Chip_UART_Read(LPC_USART, &PLAIN_TEXT[i], 1) == 0);
				//begin timeout packet measurement
				Init_TIMER();
				TIMER_Start();
			} else {
				while((TIMER_getCounter() < TIMEOUT_PACKET * PRESCALE) && (Chip_UART_Read(LPC_USART, &PLAIN_TEXT[i], 1) == 0));
			}
		} else {
			while((TIMER_getCounter() < TIMEOUT_PACKET * PRESCALE) && (Chip_UART_Read(LPC_USART, &HASH[i - 16], 1) == 0));
		}
		if ((TIMER_getCounter() > TIMEOUT_PACKET * PRESCALE) && (i != 47)) {
			i = -1;
			TIMER_Stop();
			TIMER_DeInit();
		}
	}
	TIMER_Stop();
	TIMER_DeInit();

	/* analyze the packet */

		/* nounce */
	received_nounce = (PLAIN_TEXT[15] << 8) | (PLAIN_TEXT[14]);
	if (received_nounce != nounce) {
		return 0;
	}

		/* hash */
	HMAC_SHA256(PLAIN_TEXT, 16, KEY, 32, HASH_VERIFY);
	if (ByteArray_Equal(HASH, HASH_VERIFY, 32) != 0) {
		return 0;
	}

	/* Send ACK */
	UART_SendSTR_Signed(ACK, 14, KEY, 1);

	/* increment nounce */
	nounce++;

	/* add the message (14 first bytes of plain_text) to the buffer MESSAGE */
	for (i = 0; i < 14; ++i) {

		MESSAGE[i] = PLAIN_TEXT[i];
	}

 	return 14;
}


