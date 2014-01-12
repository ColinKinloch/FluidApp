//Settings loader based on JsonCpp

#ifndef CKFD_SETTINGS
#define CKFD_SETTINGS

#include <iostream>
#include <fstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

struct Settings
{
	static boost::property_tree::ptree root;
	
	static bool loadFromFile(std::string path);
	static bool saveToFile(std::string path);
};

#endif//CKFD_SETTINGS
