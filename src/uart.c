#include <chip.h>
#include <uart.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfcrypt/src/sha256.c>

uint8_t UART_BUFFER_counter = 0;
uint8_t UART_BUFFER[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


extern volatile float kp;
extern volatile float ki;
extern volatile float kd;
extern volatile float errorSum;
extern volatile float alpha;


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

void Quit_UART()
{
}

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


uint8_t UART_Read_max_nB(uint32_t n)
{
	uint32_t counter = 0;
	char byte = 10; //10 is error (ex : no data to pull or wrong data pulled)
	while ((counter < n) && (Chip_UART_Read(LPC_USART, &byte, 1) > 0)) {
		counter++;
	}
	return byte;
}

void UART_Read_Replay()
{
	char byte;
	while (Chip_UART_Read(LPC_USART, &byte, 1) == 0);
	UART_PutCHAR(byte);
}


static void cleanBuffer()
{
	uint8_t i = 0;
	for (; i < 20; ++i) {
		UART_BUFFER[i] = 0;
	}
	UART_BUFFER_counter = 0;
	UART_PutSTR("Buffer cleaned\r\n");
}

static void UART_PutPID()
{
	UART_PutSTR(" -> Kp : ");
	UART_PutFLOAT(kp, 6);
	UART_PutSTR(", ");
	UART_PutSTR("Kd : ");
	UART_PutFLOAT(kd, 6);
	UART_PutSTR(", ");
	UART_PutSTR("Ki : ");
	UART_PutFLOAT(ki, 6);
	UART_PutSTR("\r\n");
}

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

						case 112 : 	kp = (sign) * (sum + sum2);													//p
									errorSum = 0;
									UART_PutPID();
									break;

						case 105: 	kd = (sign) * (sum + sum2);													//d
									errorSum = 0;
									UART_PutPID();
									break;

						case 100 : 	ki = (sign) * (sum + sum2);													//i
									errorSum = 0;
									UART_PutPID();
									break;

						case 97  : 	alpha = (sign) * (sum + sum2);													//a
									errorSum = 0;
									UART_PutSTR(" -> alpha = ");
									UART_PutFLOAT(alpha, 6);
									UART_PutSTR("\r\n");
									break;

						default  : 	break;																			//default
					}
					cleanBuffer();
					return;
				} else {																					//else
					switch (UART_BUFFER[0]) {																	//if its p,i,d or a

						case 112 : 	kp += (sign) * (sum + sum2);													//p
									errorSum = 0;
									UART_PutPID();
									break;

						case 105: 	kd += (sign) * (sum + sum2);													//d
									errorSum = 0;
									UART_PutPID();
									break;

						case 100 : 	ki += (sign) * (sum + sum2);													//i
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
	uint8_t byte = UART_Read_max_nB(1);
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

void UART_Test_SHA256()
{
	char UART_BUFFER_SHA256[64];	//64 bytes (32 message + 32 sha256)
	byte MESSAGE[32];
	byte HASH[32];

	uint8_t cond = 1;

	uint8_t counter = 0;
	char byte;

	while(counter < 64) {
		if (Chip_UART_Read(LPC_USART, &byte, 1) > 0) {
			UART_BUFFER_SHA256[counter] = byte;
			counter++;
		}
	}

	for (counter = 0; counter < 32; ++counter) {
		MESSAGE[counter] = UART_BUFFER_SHA256[counter];
	}

	Sha256 sha;
	wc_InitSha256(&sha);
	wc_Sha256Update(&sha, MESSAGE, 32);
	wc_Sha256Final(&sha, HASH);

	counter = 0;
	while ((counter < 32) && (cond == 1)) {
		if (HASH[counter] != UART_BUFFER_SHA256[32 + counter]) {
			cond = 0;
		}
	}

	cond == 1 ? UART_PutSTR("Success\r\n") : UART_PutSTR("Fail\r\n");
	UART_PutSTR(HASH);
}

void UART_Send_SHA256(char* buffer, uint8_t size)
{
	byte HASH[32];

	if (size > 32) {
		return;
	}
	else {
		Sha256 sha;
		wc_InitSha256(&sha);
		wc_Sha256Update(&sha, buffer, size);
		wc_Sha256Final(&sha, HASH);
		for (int i = 0; i < 32; ++i) {
			UART_PutHEX(HASH[i]);
			UART_PutSTR("\r\n");
		}
	}
}
