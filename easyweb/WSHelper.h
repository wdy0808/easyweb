#pragma once
#include "socketinfo.h"

namespace WS
{
	void getResponseKey(const char* key, char* responseKey);
	std::string getReceiveData(std::string input);
	std::string generateSendData(std::string data);
	void config(std::string inputData, CanvasInfo& data);
	std::string generateInitData(std::string data);
}
