/*
 * =====================================================================================
 *
 *       Filename:  JsonFileManager.h
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  04/17/2024 05:35:01 AM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  YOUR NAME (),
 *   Organization:
 *
 * =====================================================================================
 */

#ifndef JSON_FILE_MANAGER_H
#define JSON_FILE_MANAGER_H
//#include "ArduinoJson/Document/JsonDocument.hpp"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>

typedef enum {
  credentials_file = 0,
  wifi_configs,
  allowed_plugs,
  plug_config,
} Files;

typedef struct {
  String ssid;
  String pass;
  String ip_adr;
  bool is_set;

} WiFI_struct;

extern String file_names[];

void init_littlefs();
void dump_file(Files);
bool write_file(JsonDocument &doc, Files f);
bool readFile(JsonDocument &doc, Files f);
void stop_littlefs();
#endif
