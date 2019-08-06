#include <crypto.h>
#include <wolfcrypt/src/hmac.c>
#include <wolfcrypt/src/sha256.c>
#include <wolfcrypt/src/aes.c>
#include <wolfcrypt/src/misc.c>


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
	Aes aes;
	wc_AesSetKey(&aes, key, 32, IV, AES_DECRYPTION);
	wc_AesCbcDecrypt(&aes, plainText, cipherText, 32);
}

void Encrypt_AES258_CBC(const byte key[32], byte cipherText[32], const byte plainText[32], const byte IV[16])
{
	Aes aes;
	wc_AesSetKey(&aes, key, 32, IV, AES_ENCRYPTION);
	wc_AesCbcEncrypt(&aes, cipherText, plainText, 32);
}
