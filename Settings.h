#ifndef CKFD_SETTINGS
#define CKFD_SETTINGS

#include <iostream>
#include <fstream>
#include <jsoncpp/json/json.h>

struct Settings
{
	static Json::Value root;
	static Json::Reader reader;
	
	static bool loadFromFile(std::string path);
	static bool saveToFile(std::string path);
};

#endif//CKFD_SETTINGS
