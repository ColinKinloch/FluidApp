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

  void readMember(std::vector<std::string> path);
  std::vector<std::string> splitPath(std::string path);

public:
  Settings(std::string path);
  ~Settings();

  gchar ** listMembers();

  gboolean getNull(std::string path);
  gboolean getNull(std::vector<std::string> path);
  gboolean getBool(std::string path);
  gboolean getBool(std::vector<std::string> path);
  gint64 getInt(std::string path);
  gint64 getInt(std::vector<std::string> path);
  gdouble getDouble(std::string path);
  gdouble getDouble(std::vector<std::string> path);
  const gchar* getString(std::string path);
  const gchar* getString(std::vector<std::string> path);
};
extern Settings* settings;
