#include "stdafx.h"
#include "WSHelper.h"
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

void WS::getResponseKey(const char* key, char* responseKey)
{
	char tem[100];
	strcpy_s(tem, key);
	strcat_s(tem, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

	unsigned char digest[SHA_DIGEST_LENGTH];
	SHA1((unsigned char*)tem, strlen(tem), (unsigned char*)digest);
	size_t size = SHA_DIGEST_LENGTH * 2 + 1, count = 0;

	BIO* b64;
	BIO* bio;
	BUF_MEM* bptr;
	b64 = BIO_new(BIO_f_base64());
	bio = BIO_new(BIO_s_mem());
	bio = BIO_push(b64, bio);
	BIO_write(bio, digest, sizeof(digest));
	BIO_flush(bio);
	BIO_get_mem_ptr(bio, &bptr);
	memcpy(responseKey, bptr->data, bptr->length);
	responseKey[bptr->length - 1] = '\0';
	BIO_free_all(bio);
}
