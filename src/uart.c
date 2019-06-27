#include <chip.h>
#include <uart.h>

/* Transmit and receive ring buffers */
STATIC RINGBUFF_T txring, rxring;

/* Transmit and receive ring buffer sizes */
#define UART_SRB_SIZE 32	/* Send */
#define UART_RRB_SIZE 32	/* Receive */

/* Transmit and receive buffers */
static uint8_t rxbuff[UART_RRB_SIZE], txbuff[UART_SRB_SIZE];

void Init_UART()
{
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, (IOCON_FUNC1 | IOCON_MODE_INACT));	// RXD
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, (IOCON_FUNC1 | IOCON_MODE_INACT));	// TXD
	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, BAUDRATE);
	Chip_UART_ConfigData(LPC_USART, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_USART, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_USART);

	RingBuffer_Init(&rxring, rxbuff, 1, UART_RRB_SIZE);
	RingBuffer_Init(&txring, txbuff, 1, UART_SRB_SIZE);

	/* Enable receive data and line status interrupt */
	Chip_UART_IntEnable(LPC_USART, (UART_IER_RBRINT | UART_IER_RLSINT));

	NVIC_SetPriority(UART0_IRQn, 3);
	NVIC_EnableIRQ(UART0_IRQn);
}

void Quit_UART()
{
}

void UART_PutCHAR(char c)
{
	Chip_UART_SendRB(LPC_USART, &txring, &c, 1);
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
	uint8_t ch = 10; //10 is error (ex : no data to pull or wrong data pulled)
	uint32_t counter = 0;
	char byte;
	while ((counter < n) && (Chip_UART_ReadRB(LPC_USART, &rxring, &byte, 1) > 0)) {
		counter++;
	}
	ch = discoverOne(byte);
	return ch;
}

void UART_IRQHandler(void)
{
	Chip_UART_IRQRBHandler(LPC_USART, &rxring, &txring);
}
