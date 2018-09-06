#include "stdafx.h"
#include "WSHelper.h"
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <cstring>

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

std::string WS::getReceiveData(char* input, size_t bytes)
{
	int payloadLen = input[1] & 127, maskStart = 2;
	char mask[5];
	if (payloadLen == 126)
		maskStart = 4;
	else if (payloadLen == 127)
		maskStart = 10;

	strncpy_s(mask, 5, input + maskStart, 4);
	rsize_t dataLen = bytes - maskStart - 4;
	input += maskStart + 4;
	for (size_t i = 0; i < dataLen; ++i)
		input[i] = input[i] ^ mask[i % 4];

	return std::string(input, dataLen);
}

void WS::generateSendData(char* output)
{
	std::string data(output);
	size_t dataLen = data.size(), dataStart = 2;
	output[0] = output[0] & 0 | 0x81;
	if (dataLen < 126)
		output[1] = output[1] & 0 | dataLen;
	else if (dataLen <= 0xFFFF)
	{
		dataStart = 4;
		output[1] = output[1] & 0 | 126;
		char* len = (char*)&dataLen;
		output[2] = len[1];
		output[3] = len[0];
	}
	else
	{
		dataStart = 10;
		output[1] = output[1] & 0 | 127;
		char* len = (char*)&dataLen;
		for (int i = 0; i < 8; i++)
			output[i + 2] = len[7 - i];
	}
	output[dataStart] = '\0';
	char t = output[1];
	strcat_s(output, dataLen + 3, data.c_str());
}