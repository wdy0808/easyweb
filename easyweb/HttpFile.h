#pragma once
#include <string>

namespace Http
{
	bool fileExist(std::string url);
	bool fileReadable(std::string url);
	std::string getFile(std::string url);
}