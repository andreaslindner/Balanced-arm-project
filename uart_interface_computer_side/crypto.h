#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdio.h>
#include <stdint.h>
#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/hmac.h>
#include <wolfssl/wolfcrypt/aes.h>

void HMAC_SHA256(const byte MESSAGE[], const uint8_t sizeMESSAGE, const byte KEY[], const uint8_t sizeKEY, byte HASH[32]);
void HASH_SHA256(const byte MESSAGE[], const uint8_t sizeMESSAGE, byte HASH[32]);
void Decrypt_AES258_CBC(const byte key[32], byte plainText[32], const byte cipherText[32], const byte IV[16]);
void Encrypt_AES258_CBC(const byte key[32], byte cipherText[32], const byte plainText[32], const byte IV[16]);
void Random_Generator(byte* buffer, const uint8_t sizeBuffer);

#endif