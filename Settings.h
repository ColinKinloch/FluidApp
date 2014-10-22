//Settings loader based on JsonCpp

#pragma once

#include <glib.h>
#include <json-glib/json-glib.h>
#include <vector>
#include <string>


class Settings
{
  JsonParser* parser;
  JsonReader* reader;
  JsonNode* root;

  JsonGenerator* generator;

  std::string filePath;

  JsonNode* getNode(const char* path);
  JsonNode* queryNode(const char* path);

public:
  Settings(std::string path);
  ~Settings();

  gchar ** listMembers();

  gboolean save();
  gboolean save(const char* path);

  void setValue(const char* path, const GValue* value);
  gboolean getNull(const char* path);
  void setNull(const char* path);
  gboolean getBool(const char* path);
  void setBool(const char* path, gboolean value);
  gint64 getInt(const char* path);
  void setInt(const char* path, gint64 value);
  gdouble getDouble(const char* path);
  void setDouble(const char* path, gdouble value);
  const gchar* getString(const char* path);
  void setString(const char* path, const gchar* value);
};
extern Settings* settings;
