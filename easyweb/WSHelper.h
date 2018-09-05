#pragma once

namespace WS
{
	void getResponseKey(const char* key, char* responseKey);
	std::string getReceiveData(char* input, size_t bytes);
	void generateSendData(char* output);
}
