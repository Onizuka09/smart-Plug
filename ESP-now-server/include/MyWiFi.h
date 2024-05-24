#ifndef MYWIFI_H
#define MYWIFI_H
#include "JsonFileManager.h"
#include "state_machine.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#define maxSSID_res 10

class MyWifi {
private:
  const int num_trials = 20;

  const char *ESP_ssid = "ESP32";

public:
  WiFI_struct wifi_data;
  IPAddress APIP;
  MyWifi();
  ~MyWifi() {}
  state_machine connect_wifi(bool read_cf);

  bool scan_wifi(JsonDocument &jdoc);
  state_machine saveWiFiConfigs();
  bool create_AP();

private:
  void getWiFiValues(JsonDocument &doc);
  bool readWificonfigs();
  void setWiFiJsondata(JsonDocument &doc);
};
#endif
