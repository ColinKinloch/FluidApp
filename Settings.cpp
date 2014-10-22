#include "Settings.h"
#include <iostream>

Settings* settings = nullptr;

Settings::Settings(std::string path){
  filePath = path;
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

JsonNode* Settings::queryNode(const char *path)
{
  return json_path_query(path, root, NULL);
}
JsonNode* Settings::getNode(const char* path)
{
  JsonNode* node = queryNode(path);
  JsonArray* array = json_node_get_array (node);
  return json_array_get_element(array, 0);
}

gboolean Settings::save(){
  return save(filePath.c_str());
}
gboolean Settings::save(const char* path){
  generator = json_generator_new();
  json_generator_set_root(generator, root);

  return json_generator_to_file(generator, path, NULL);
}

void Settings::setValue(const char *path, const GValue* value)
{
  JsonNode* node = getNode(path);
  json_node_set_value(node, value);
}

gboolean Settings::getNull(const char* path){
  JsonNode* node = getNode(path);
  return  json_node_is_null(node);
}
void Settings::setNull(const char* path)
{
  JsonNode* node = getNode(path);
  json_node_init_null(node);
  json_node_free(node);
}

gboolean Settings::getBool(const char* path){
  JsonNode* node = getNode(path);
  gboolean value = json_node_get_boolean (node);
  json_node_free(node);
  return value;
}
void Settings::setBool(const char* path, gboolean value){
  JsonNode* node = getNode(path);
  json_node_set_boolean(node, value);
  json_node_free(node);
}

gint64 Settings::getInt(const char* path){
  JsonNode* node = getNode(path);
  gint64 value = json_node_get_int (node);
  json_node_free(node);
  return value;
}
void Settings::setInt(const char* path, gint64 value){
  JsonNode* node = getNode(path);
  json_node_set_int (node, value);
  json_node_free(node);
}

gdouble Settings::getDouble(const char* path){
  JsonNode* node = getNode(path);
  gdouble value = json_node_get_double (node);
  json_node_free(node);
  return value;
}
void Settings::setDouble(const char* path, gdouble value){
  JsonNode* node = getNode(path);
  json_node_set_double (node, value);
  json_node_free(node);
}

const gchar* Settings::getString(const char* path){
  JsonNode* node = getNode(path);
  const gchar* value = json_node_get_string (node);
  json_node_free(node);
  return value;
}
void Settings::setString(const char* path, const char* value){
  JsonNode* node = getNode(path);
  json_node_set_string(node, value);
  json_node_free(node);
}
