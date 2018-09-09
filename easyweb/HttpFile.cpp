#include "stdafx.h"
#include "HttpFile.h"
#include <io.h>
#include <fstream>  
#include <streambuf> 

bool Http::fileExist(std::string url)
{
	std::string address = "../../webpage" + url;
	return _access(address.c_str(), 0) == 0;
}

bool Http::fileReadable(std::string url)
{
	std::string address = "../../webpage" + url;
	return _access(address.c_str(), 4) == 0;
}

std::string Http::getFile(std::string url)
{
	std::string address = "../../webpage" + url;
	std::ifstream t(address);
	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	return str;
}