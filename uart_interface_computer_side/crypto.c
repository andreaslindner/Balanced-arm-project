#include "crypto.h"
#include <wolfssl/options.h>
#include <wolfssl/wolfcrypt/sha256.h>
#include <wolfssl/wolfcrypt/hmac.h>
#include <wolfssl/wolfcrypt/aes.h>

void HMAC_SHA256(const byte MESSAGE[], const uint8_t sizeMESSAGE, const byte KEY[], const uint8_t sizeKEY, byte HASH[32])
{
	Hmac hmac;
	wc_HmacSetKey(&hmac, SHA256, KEY, sizeKEY);
	wc_HmacUpdate(&hmac, MESSAGE, sizeMESSAGE);
	wc_HmacFinal(&hmac, HASH);
}

void HASH_SHA256(const byte MESSAGE[], const uint8_t sizeMESSAGE, byte HASH[32])
{
	Sha256 sha;
	wc_InitSha256(&sha);
	wc_Sha256Update(&sha, MESSAGE, sizeMESSAGE);
	wc_Sha256Final(&sha, HASH);
}

void Decrypt_AES258_CBC(const byte key[32], byte plainText[32], const byte cipherText[32], const byte IV[16])
{
	int ret;
	Aes aes;
	ret = wc_AesSetKey(&aes, key, 32, IV, AES_DECRYPTION);
	if (ret != 0) {
		printf("setKey failed\n");
	}
	ret = wc_AesCbcDecrypt(&aes, plainText, cipherText, 32);
	if (ret != 0) {
		printf("decrypt failed\n");
	}
}

void Encrypt_AES258_CBC(const byte key[32], byte cipherText[32], const byte plainText[32], const byte IV[16])
{
	int ret;
	Aes aes;
	ret = wc_AesSetKey(&aes, key, 32, IV, AES_ENCRYPTION);
	if (ret != 0) {
		printf("setKey failed\n");
	}
	ret = wc_AesCbcEncrypt(&aes, cipherText, plainText, 32);
	if (ret != 0) {
		printf("encrypt failed\n");
	}
}

void Random_Generator(byte* buffer, const uint8_t sizeBuffer)
{
	RNG rng;
	wc_InitRng(&rng);
	wc_RNG_GenerateBlock(&rng, buffer, sizeBuffer);
	wc_FreeRng(&rng);
}
