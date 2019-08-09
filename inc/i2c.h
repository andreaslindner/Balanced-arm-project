#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

#define FAST_MODE_PLUS	0

#define BUFSIZE             64
#define MAX_TIMEOUT		0x000FFFFF

#define I2CMASTER		0x01
#define I2CSLAVE		0x02

#define PCF8594_ADDR	0xA0
#define READ_WRITE		0x01

#define RD_BIT			0x01

#define I2C_IDLE			0
#define I2C_STARTED			1
#define I2C_RESTARTED		2
#define I2C_REPEATED_START	3
#define DATA_ACK			4
#define DATA_NACK			5
#define I2C_BUSY              6
#define I2C_NO_DATA           7
#define I2C_NACK_ON_ADDRESS   8
#define I2C_NACK_ON_DATA      9
#define I2C_ARBITRATION_LOST  10
#define I2C_TIME_OUT          11
#define I2C_OK                12

#define I2CONSET_I2EN       (0x1<<6)  /* I2C Control Set Register */
#define I2CONSET_AA         (0x1<<2)
#define I2CONSET_SI         (0x1<<3)
#define I2CONSET_STO        (0x1<<4)
#define I2CONSET_STA        (0x1<<5)

#define I2CONCLR_AAC        (0x1<<2)  /* I2C Control clear Register */
#define I2CONCLR_SIC        (0x1<<3)
#define I2CONCLR_STAC       (0x1<<5)
#define I2CONCLR_I2ENC      (0x1<<6)

#define I2DAT_I2C			0x00000000  /* I2C Data Reg */
#define I2ADR_I2C			0x00000000  /* I2C Slave Address Reg */
#define I2SCLH_SCLH			0x00000180  /* I2C SCL Duty Cycle High Reg */
#define I2SCLL_SCLL			0x00000180  /* I2C SCL Duty Cycle Low Reg */
#define I2SCLH_HS_SCLH		0x00000015  /* Fast Plus I2C SCL Duty Cycle High Reg */
#define I2SCLL_HS_SCLL		0x00000015  /* Fast Plus I2C SCL Duty Cycle Low Reg */

/**
 * @brief	Init and configure I2C bus
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
extern uint32_t I2CInit( uint32_t I2cMode );

/**
 * @brief	Read numerous bytes from a given address register of a device connected to the bus
 * @param	dev_id : address of the slave device
 * 			reg : address of the register
 * 			n : number of bytes to read from the given address register
 * @return	Nothing
 * @note	Blocking read
 */
void I2C_Read_Blocking(uint8_t dev_id, uint8_t reg, int n);

/**
 * @brief	Read numerous bytes from a given address register of a device connected to the bus
 * @param	dev_id : address of the slave device
 * 			reg : address of the register
 * 			n : number of bytes to read from the given address register
 * @return	Nothing
 * @note	Not blocking read
 */
void I2C_Read_nBlocking(uint8_t dev_id, uint8_t reg, int n);

/**
 * @brief	Write numerous bytes into a given address register of a device connected to the bus
 * @param	dev_id : address of the slave device
 * 			reg : address of the register
 * 			n : number of bytes to write from the given address register
 * 			ba : byte address of the byte which will be written into the register (it's uint8_t so this has to be modified if you want to write numerous bytes)
 * @return	Nothing
 * @note	Blocking write
 */
uint8_t I2C_Write_Blocking(uint8_t dev_id, uint8_t reg, uint8_t* ba, int n);

/**
 * @brief	Write numerous bytes into a given address register of a device connected to the bus
 * @param	dev_id : address of the slave device
 * 			reg : address of the register
 * 			n : number of bytes to write from the given address register
 * 			ba : byte address of the byte which will be written into the register
 * @return	Nothing
 * @note	Blocking write
 */
uint8_t I2C_Write_Blocking_1B(uint8_t dev_id, uint8_t reg, uint8_t b);

/**
 * @brief	Deal with I2C protocol for one answer
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
void I2CDriver();

/**
 * @brief	Handler of the i2c interrupt which is triggered each time a slave write to us
 * @param	Nothing
 * @return	Nothing
 * @note	Nothing
 */
extern void I2C_IRQHandler( void );
#endif
