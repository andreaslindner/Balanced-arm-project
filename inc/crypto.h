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


void HMAC_SHA256(byte MESSAGE[], uint8_t sizeMESSAGE, byte KEY[], uint8_t sizeKEY, byte HASH[32]);
void HASH_SHA256(byte MESSAGE[], uint8_t sizeMESSAGE, byte HASH[32]);

#endif /* CRYPTO_H_ */
