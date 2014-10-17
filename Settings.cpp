#include "Settings.h"
#include <iostream>

Settings* settings = nullptr;

Settings::Settings(std::string path){
  GError* error;
  parser = json_parser_new();
  error = nullptr;
  json_parser_load_from_file(parser, path.c_str(), &error);
  if (error)
  {
          g_error_free(error);
          g_object_unref(parser);
          std::cout<<"settings error"<<std::endl;
  }
  root = json_parser_get_root(parser);
  reader = json_reader_new(root);
}
Settings::~Settings(){
  g_object_unref(root);
  g_object_unref(reader);
  g_object_unref(parser);
}

gchar ** Settings::listMembers(){
  json_reader_end_member(reader);
  return json_reader_list_members(reader);
}
std::vector<std::string> Settings::splitPath(std::string path){
  size_t pos = 0;
  const std::string delim = ".";
  std::vector<std::string> sPath;
  std::string curr;
  while((pos = path.find(delim)) != std::string::npos){
    curr = path.substr(0, pos);
    sPath.push_back(curr);
    path.erase(0, pos+delim.length());
  }
  sPath.push_back(path);
  return sPath;
}

void Settings::readMember(std::vector<std::string> path){
  json_reader_set_root(reader, root);
  for(auto it = path.begin(); it != path.end(); ++it)
  {
    json_reader_read_member(reader, it->c_str());
  }
}

gboolean Settings::getNull(std::string path){
  return getNull(std::vector<std::string>({path}));
}
gboolean Settings::getNull(std::vector<std::string> path){
  readMember(path);
  return json_reader_get_null_value(reader);
}

gboolean Settings::getBool(std::string path){
  return getBool(splitPath(path));
}
gboolean Settings::getBool(std::vector<std::string> path){
  readMember(path);
  return json_reader_get_boolean_value(reader);
}

gint64 Settings::getInt(std::string path){
  return getInt(splitPath(path));
}
gint64 Settings::getInt(std::vector<std::string> path){
  readMember(path);
  return json_reader_get_int_value(reader);
}

gdouble Settings::getDouble(std::string path){
  return getDouble(std::vector<std::string>({path}));
}
gdouble Settings::getDouble(std::vector<std::string> path){
  readMember(path);
  return json_reader_get_double_value(reader);
}


const gchar* Settings::getString(std::string path){
  json_reader_end_member(reader);
  json_reader_read_member(reader, path.c_str());
  return json_reader_get_string_value(reader);
}
