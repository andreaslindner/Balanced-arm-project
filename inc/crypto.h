/*
 * crypto.h
 *
 *  Created on: 29 juil. 2019
 *      Author: bcoste
 */

#ifndef CRYPTO_H_
#define CRYPTO_H_

#include <stdint.h>
#include <wolfssl/options.h>
#include <wolfssl_user_setting.h>			//modify this file to change compilation parameters of the wolfssl library
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/hmac.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/misc.h>

/**
 * @brief	Compute the hash of the message
 * @param	MESSAGE : address of the buffer where the message is stored
 * 			sizeMESSAGE : size of the message, should be less than 32 bytes
 * 			HASH : address to the buffer where the 32-byte hash is stored
 * @return	Nothing
 * @note	The hash is performed using sha256
 */
void HASH_SHA256(const byte MESSAGE[], const uint8_t sizeMESSAGE, byte HASH[32]);

/**
 * @brief	Compute the HMAC of the message
 * @param	MESSAGE : address of the buffer where the message is stored
 * 			sizeMESSAGE : size of the message, should be less than 32 bytes
 * 			KEY : 32-byte or less key
 * 			sizeKEY : size of the key
 * 			HASH : address to the buffer where the 32-byte hash is stored
 * @return	Nothing
 * @note	The HMAC is performed using sha256
 */
void HMAC_SHA256(const byte MESSAGE[], const uint8_t sizeMESSAGE, const byte KEY[], const uint8_t sizeKEY, byte HASH[32]);

/**
 * @brief	Encrypt the message using AES 258 - CBC
 * @param	key : 32-byte key used to encrypt the plainText
 * 			cipherText : address of the buffer where the encrypted message will be stored, must be at least 32-byte long
 * 			plainText : address of the buffer where the plain message is stored
 * 			IV : address of the buffer where the 16-byte IV is stored
 * @return	Nothing
 * @note	Nothing
 */
void Encrypt_AES258_CBC(const byte key[32], byte cipherText[32], const byte plainText[32], const byte IV[16]);

/**
 * @brief	Decrypt the message using AES 258 - CBC
 * @param	key : 32-byte key used to encrypt the plainText
 * 			plainText : address of the buffer where the plain message will be stored
 * 			cipherText : address of the buffer where the encrypted message is stored, must be at least 32-byte long
 * 			IV : address of the buffer where the 16-byte IV is stored
 * @return	Nothing
 * @note	Nothing
 */
void Decrypt_AES258_CBC(const byte key[32], byte plainText[32], const byte cipherText[32], const byte IV[16]);

#endif /* CRYPTO_H_ */
