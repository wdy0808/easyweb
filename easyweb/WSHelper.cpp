#include "stdafx.h"
#include "WSHelper.h"
#include <openssl/sha.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include "easyjson.h"

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

std::string WS::getReceiveData(std::string input)
{
	int payloadLen = input[1] & 127, maskStart = 2;
	if (payloadLen == 126)
		maskStart = 4;
	else if (payloadLen == 127)
		maskStart = 10;

	std::string mask = input.substr(maskStart, 4); 
	input = input.substr(maskStart + 4);
	for (size_t i = 0; i < input.size(); ++i)
		input[i] = input[i] ^ mask[i % 4];
	return input;
}

std::string WS::generateSendData(std::string data)
{
	std::string output(2, ' ');
	size_t dataLen = data.size();
	output[0] = output[0] & 0 | 0x81;
	if (dataLen < 126)
		output[1] = output[1] & 0 | dataLen;
	else if (dataLen <= 0xFFFF)
	{
		output += std::string(2, ' ');
		output[1] = output[1] & 0 | 126;
		char* len = (char*)&dataLen;
		output[2] = len[1];
		output[3] = len[0];
	}
	else
	{
		output += std::string(8, ' ');
		output[1] = output[1] & 0 | 127;
		char* len = (char*)&dataLen;
		for (int i = 0; i < 8; i++)
			output[i + 2] = len[7 - i];
	}
	return output + data;
}

void WS::config(std::string inputData, CanvasInfo& data)
{
	EasyJson json(inputData);
	data.username = json.getString("username");
	data.canvasname = json.getString("canvasname");
	if (data.canvasname.empty())
	{
		data.index = true;
		return;
	}
	data.index = false;
	data.selfname = json.getString("selfname");
}

std::string WS::generateInitData(std::string data)
{
	return "{\"type\":\"init\",\"data\":" + data + "}";
}