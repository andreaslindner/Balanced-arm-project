#include "crypto.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <libserialport.h>

#define TIMEOUT_ACK 10
#define TIMEOUT_PACKET 2
#define REPLAY

const byte MOTHER_KEY[32] = "12345678912345678912345678912345";
volatile byte DERIVATE_KEY[32];
volatile uint8_t UART_connected;
volatile uint16_t nounce = 0;
volatile struct sp_port * port;
const byte ACK[14] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static void string2hexString(byte* input, int size_input, void* output)
{
    int loop;
    int i; 
    
    i=0;
    loop=0;
    
    while (loop < size_input)
    {
        sprintf((char*)(output+i),"%02X", input[loop]);
        loop+=1;
        i+=2;
    }
    // insert NULL at the end of the output string
}

static uint8_t Init_UART()
{
	struct sp_port ** ports;
	sp_list_ports(&ports);
	int i = 0;
	while (ports[i] != NULL)
	{
		printf("port name: %s\n", sp_get_port_name(ports[i]));
		i++;
	}
	port = ports[0];
	if(!port){
		printf("Port is NULL\n");
		return 1;
	}
	if (SP_OK != sp_open((struct sp_port *)port, SP_MODE_READ | SP_MODE_WRITE))
	{
		printf("Can't open the port\n");
		return 2;
	}
	printf("Port open : %s\n", sp_get_port_name((struct sp_port *)port));
	sp_set_baudrate((struct sp_port *)port, 9600);
	sp_set_bits((struct sp_port *)port, 8);
	sp_set_flowcontrol((struct sp_port *)port, SP_FLOWCONTROL_NONE);
	sp_set_parity((struct sp_port *)port, SP_PARITY_NONE);
	sp_set_stopbits((struct sp_port *)port, 1);
	return 0;
}

static void UART_Send_nB(byte *message, const uint8_t sizeMessage) {
	sp_blocking_write((struct sp_port *) port, (char *)message, sizeMessage, 0);
}

static int UART_Read_Blocking(byte *message, const uint8_t sizeMessage, int timeout)
{
	return (sp_blocking_read((struct sp_port *) port, message, sizeMessage, timeout));
}

static int UART_Read_non_Blocking(byte *message, const uint8_t sizeMessage)
{
	return ( sp_nonblocking_read((struct sp_port *) port, message, sizeMessage) );
}

static uint8_t ByteArray_Equal(const byte one[], const byte two[], uint8_t size)
{
	for (uint8_t i = 0; i < size; ++i) {
		if (one[i] != two[i]) {
			return 1;
		}
	}
	return 0;
}

static uint8_t Wait_ACK(const byte KEY[32])
{
	uint8_t counter = 0;
	uint8_t timeout = 1;
	int ret;
	byte PLAIN_TEXT[16];
	byte HASH[32];
	byte HASH_VERIFY[32];
	uint16_t received_nounce;
	byte output[64];

	clock_t start = clock();

	/* while we don't timeout, we try to read ACK */
	while (((clock() - start) / CLOCKS_PER_SEC) < TIMEOUT_ACK)  {

		if (counter < 16) {
			ret = UART_Read_non_Blocking(&PLAIN_TEXT[counter], 16-counter);
			if (ret < 0) {
				printf("Error while reading ACK PLAIN\n");
			} else {
				counter += ret;
			}
		} else {
			ret = UART_Read_non_Blocking(&HASH[counter - 16], 48 - counter);
			if (ret < 0) {
				printf("Error while reading ACK HASH\n");
			} else {
				counter += ret;
				if (counter == 48) {
					timeout = 0;
					break;
				}
			}
		}
	}

	if (timeout == 1) {
		return 1;
	}

	/* we didn't timeout, we have to verify the message now */
	if (ByteArray_Equal(PLAIN_TEXT, ACK, 14) != 0) {
		string2hexString(PLAIN_TEXT, 16, output);
		printf("PLAIN_TEXT : %s\n", output);
		string2hexString((byte *)ACK, 14, output);
		printf("ACK : %s\n", output);
		return 2;
	}

	/*ACK is ok, verify nouce now */
	received_nounce = (PLAIN_TEXT[15] << 8) | (PLAIN_TEXT[14]);
	if (received_nounce != nounce) {
		printf("My nounce : %d\n Nounce received : %d\n", nounce, received_nounce);
		return 3;
	}

	/* Now we verify the HASH */
	HMAC_SHA256(PLAIN_TEXT, 16, KEY, 32, HASH_VERIFY);
	if (ByteArray_Equal(HASH, HASH_VERIFY, 32) != 0) {
		string2hexString(HASH, 32, output);
		printf("HASH_RE : %s\n", output);
		string2hexString(HASH_VERIFY, 32, output);
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
	int ret;

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
			ret = Wait_ACK(KEY);
			if (ret != 0) {
				return ret;
			}
			nounce++;
		}
	}
	return 0;
}

static uint16_t UART_ReceiveSTR_Signed(byte MESSAGE[], const byte KEY[32])
{
	byte HASH[32];
	byte PLAIN_TEXT[16];
	int8_t i;
	byte HASH_VERIFY[32];
	uint16_t received_nounce;
	int ret = 0;
	
	/* receive one packet of 48 bytes */
	while (ret != 32) {
		
		UART_Read_Blocking(PLAIN_TEXT, 1, 0);
		ret = UART_Read_Blocking(&PLAIN_TEXT[1], 15, TIMEOUT_PACKET * 500);
		if (ret == 15){
			ret = UART_Read_Blocking(HASH, 32, TIMEOUT_PACKET * 500);
		}
	}

	/* analyze the packet */

		/* nounce */
	received_nounce = (PLAIN_TEXT[15] << 8) | (PLAIN_TEXT[14]);
	if (received_nounce != nounce) {
		printf("Nounce not equal : %d vs %d avec : %d first byte et %d second byte\n", received_nounce, nounce, PLAIN_TEXT[15], PLAIN_TEXT[14]);
		return 0;
	}

		/* hash */
	HMAC_SHA256(PLAIN_TEXT, 16, KEY, 32, HASH_VERIFY);
	if (ByteArray_Equal(HASH, HASH_VERIFY, 32) != 0) {
		printf("Hash not equal\n");
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


static uint8_t Handshake(byte DERIVATE_KEY[32])
{
	int ret;
	byte IV[16], MESSAGE[32], HASH_PLAIN[32], HASH_CIPHER[32];
	/*char keyHex[64], messageHex[64], IVHex[32], hashHex[64], hashCHex[64], IV2Hex[32];*/
	uint8_t i = 0;


	/* Generate random IV */
	Random_Generator(IV, 16);

	/*string2hexString(IV, 16, IVHex);
	printf("IV : %s\n", IVHex);*/

	/* Generate random number */
	Random_Generator(MESSAGE, 32);

	/*string2hexString(MESSAGE, 32, messageHex);
	printf("Message : %s\n", messageHex);*/

	/* Hash message */
	HASH_SHA256(MESSAGE, 32, HASH_PLAIN);

	/*string2hexString(HASH_PLAIN, 32, hashHex);
	printf("Hash_plain : %s\n", hashHex);*/

	/* Encrypt the hash with mother key*/
	Encrypt_AES258_CBC(MOTHER_KEY, HASH_CIPHER, HASH_PLAIN, IV);

	/*string2hexString(HASH_CIPHER, 32, hashCHex);
	printf("Hash_cipher : %s\n", hashCHex);*/



	/* Send IV||MESSAGE||CIPHER_HASH */
	UART_Send_nB(IV, 16);
	UART_Send_nB(MESSAGE, 32);
	UART_Send_nB(HASH_CIPHER, 32);
	
	while ((i != 16) && (IV[i] == 255)) {
		IV[i] = 0;
		++i;
	}
	if (i != 16) {
		IV[i] += 1;
	}
	/*string2hexString(IV, 16, IV2Hex);
	printf("IV + 1 : %s\n", IV2Hex);*/

	/* Derivate Key */
	Encrypt_AES258_CBC(MOTHER_KEY, DERIVATE_KEY, MESSAGE, IV);

	/*string2hexString(DERIVATE_KEY, 32, keyHex);
	printf("Derivate_Key : %s\n", keyHex);*/

	ret = Wait_ACK(DERIVATE_KEY);
	if (ret != 0) {
		return ret;
	}
	nounce=1;
	UART_connected = 1;
	return 0;
}

void handle_sigint(int sig)
{
	sp_close((struct sp_port *)port);
	printf("Program stopped\n");
	exit(1);
}

void handle_sigtstp(int sig) 
{
	printf("\n");
	int ret;
	
	#ifdef REPLAY
	char str[64];
    fgets(str, sizeof(str), stdin);
    if (strlen(str) > 0) {
    	UART_Send_nB(str, strlen(str));
    }
    #endif

    #ifdef CRYPT
	if (!UART_connected) {
		ret = Handshake((byte*)DERIVATE_KEY);
		if ( ret != 0) {
			printf("Handshake failed : %d\n", ret);
		} else {
			printf("Handshake succeeded\n");
		}
	} else {
		char str[64];
	    fgets(str, sizeof(str), stdin);
	    if (strlen(str) > 1) {
	    	ret = UART_SendSTR_Signed((byte *)str, ((strlen(str) - 1) > 14 ? 14 : (strlen(str) -1)), (byte *)DERIVATE_KEY, 0);
	    	if (ret != 0) {
	    		printf("Message not sent : error %d\n", ret);
	    	}
   		}
	}
	#endif
		

} 


int main(int argc, char *argv[])
{
	int ret;
	byte MESSAGE[64];

	signal(SIGTSTP, handle_sigtstp);
	signal(SIGINT, handle_sigint);
	wolfCrypt_Init();
	if (Init_UART() != 0) {
		exit(0);
	}	

	while (1)
	{
		#ifdef CRYPT
		ret = UART_ReceiveSTR_Signed(MESSAGE, (byte *)DERIVATE_KEY);
		for (uint8_t i = 0; i < ret; ++i) {
			
			if (MESSAGE[i] != 0) {
				printf("%c", MESSAGE[i]);
			}
			if (i == (ret-1)) {
				printf("\nNounce : %d\n", nounce);
			}
		}
		#endif

		#ifdef REPLAY
		UART_Read_Blocking(MESSAGE, 1, 0);
		fprintf(stderr, "%c", (char)MESSAGE[0]);
		#endif

	}

}