#pragma once
#include <vector>

namespace Http
{
	void split(std::string& str, char seperate, std::vector<std::string>& sto);
	std::string getGMTstring();
}