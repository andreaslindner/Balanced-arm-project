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
//#include <wolfssl/wolfcrypt/settings.h>
#include <wolfssl_user_setting.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/hmac.h>
#include <wolfssl/wolfcrypt/aes.h>
#include <wolfssl/wolfcrypt/misc.h>

void HMAC_SHA256(const byte MESSAGE[], const uint8_t sizeMESSAGE, const byte KEY[], const uint8_t sizeKEY, byte HASH[32]);
void HASH_SHA256(const byte MESSAGE[], const uint8_t sizeMESSAGE, byte HASH[32]);
void Decrypt_AES258_CBC(const byte key[32], byte plainText[32], const byte cipherText[32], const byte IV[16]);
void Encrypt_AES258_CBC(const byte key[32], byte cipherText[32], const byte plainText[32], const byte IV[16]);

#endif /* CRYPTO_H_ */
