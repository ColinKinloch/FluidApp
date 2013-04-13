#include "Settings.h"

Json::Value Settings::root;
Json::Reader Settings::reader;

bool Settings::loadFromFile(std::string path)
{
	std::ifstream file(path);
	if(!file.good())
	 std::cerr<<"Settings: file not good"<<std::endl;
	std::string fileStr((std::istreambuf_iterator<char>(file)),
	 std::istreambuf_iterator<char>());
	
	reader.parse(fileStr, root);
	return true;
}
bool Settings::saveToFile(std::string path)
{
	
	return true;
}
