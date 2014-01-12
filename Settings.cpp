#include "Settings.h"

boost::property_tree::ptree Settings::root;

bool Settings::loadFromFile(std::string path)
{
	read_json(path, root);
	return true;
}
bool Settings::saveToFile(std::string path)
{
	
	return true;
}
