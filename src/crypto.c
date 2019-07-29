#include <crypto.h>
#include <wolfcrypt/src/hmac.c>
#include <wolfcrypt/src/sha256.c>

void HMAC_SHA256(byte MESSAGE[], uint8_t sizeMESSAGE, byte KEY[], uint8_t sizeKEY, byte HASH[32])
{
	Hmac hmac;
	wc_HmacSetKey(&hmac, SHA256, KEY, sizeKEY);
	wc_HmacUpdate(&hmac, MESSAGE, sizeMESSAGE);
	wc_HmacFinal(&hmac, HASH);
}

void HASH_SHA256(byte MESSAGE[], uint8_t sizeMESSAGE, byte HASH[32])
{
	Sha256 sha;
	wc_InitSha256(&sha);
	wc_Sha256Update(&sha, MESSAGE, sizeMESSAGE);
	wc_Sha256Final(&sha, HASH);
}
