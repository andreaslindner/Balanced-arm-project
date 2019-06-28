#include <chip.h>
#include <uart.h>

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

/* analyse the received byte to check if he's between 1 and 7 */
static uint8_t discoverOne(char byte)
{
	uint8_t res;
	if (('1' <= byte) && (byte <= '7')) {
		res = (uint8_t) (byte - '0');
	} else {
		res = 10;
	}
	return(res);
}

uint8_t UART_Read_max_nB(uint32_t n)
{
	uint8_t ch;
	uint32_t counter = 0;
	char byte = 10; //10 is error (ex : no data to pull or wrong data pulled)
	while ((counter < n) && (Chip_UART_Read(LPC_USART, &byte, 1) > 0)) {
		counter++;
	}
	ch = discoverOne(byte);
	return ch;
}

