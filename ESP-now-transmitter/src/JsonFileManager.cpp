/*
 * =====================================================================================
 *
 *       Filename:  JsonFileManager.cpp
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/17/2024 05:50:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#include "JsonFileManager.h"
/*
 * mac.json
 *
 * */
String file_names[] = {
    "/credentials.json",
    "/wifi.json",
    "/allowed_plugs.json",
    "/plug_config.json",
};
void init_littlefs() {

  if (!LittleFS.begin()) {
    Serial.println("failed to init littleFS");
    return;
  }
}
void stop_littlefs() { LittleFS.end(); }

bool readFile(JsonDocument &doc, Files f) {
  File file = LittleFS.open(file_names[f], "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    file.close();
    return false;
  }

  String jdata = file.readString();
  file.close();

  DeserializationError err = deserializeJson(doc, jdata);
  if (err) {
    Serial.println("Failed parsing JSON");
    return false;
  }

  return true;
}
bool write_file(JsonDocument &doc, Files f) {
  File file = LittleFS.open(file_names[f], FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for reading");
    file.close();
    return false;
  }

  String wdata;
  serializeJson(doc, wdata);
  // Serial.println(wdata);
  const char *msg = wdata.c_str();
  file.print(msg);
  file.close();
  return true;
}
void dump_file(Files f) {
  File file = LittleFS.open(file_names[f], FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading");
    file.close();
    return;
  }
  String fcontent = file.readString();
  Serial.println(fcontent);
  file.close();
  fcontent.clear();
}
