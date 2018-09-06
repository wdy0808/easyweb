#pragma once

namespace WS
{
	void getResponseKey(const char* key, char* responseKey);
	std::string getReceiveData(std::string input);
	std::string generateSendData(std::string data);
}
