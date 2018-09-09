#include "stdafx.h"
#include "HttpString.h"
#include <ctime>
#include <cstdlib>

void Http::split(std::string& str, char seperate, std::vector<std::string>& sto)
{
	int index = str.find(seperate, 1), last = -1;
	while (index != std::string::npos)
	{
		sto.push_back(str.substr(last + 1, index - last));
		last = index;
		index = str.find(seperate, last + 1);
	}
	sto.push_back(str.substr(last));
}

std::string Http::getGMTstring()
{
	time_t rawTime;
	struct tm* timeInfo;
	char szTemp[30] = { 0 };
	time(&rawTime);
	timeInfo = gmtime(&rawTime);
	strftime(szTemp, sizeof(szTemp), "%a, %d %b %Y %H:%M:%S GMT", timeInfo);
	return std::string(szTemp);
}