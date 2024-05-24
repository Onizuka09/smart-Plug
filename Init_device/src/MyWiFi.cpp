#include "MyWiFi.h"
//#include "ArduinoJson/Document/StaticJsonDocument.hpp"
#include "HardwareSerial.h"
#include "IPAddress.h"
#include "JsonFileManager.h"
#include "WString.h"
//#include "WiFiAP.h"
#include "WiFiType.h"
#include "esp32-hal.h"
#include "esp_wifi_types.h"
#include "state_machine.h"
#include <WiFi.h>
#include <cstdio>

MyWifi::MyWifi() : APIP(172, 0, 0, 1) {
  wifi_data.ip_adr = "";
  wifi_data.is_set = false;
  wifi_data.pass = "";
  wifi_data.ssid = "";
}

bool MyWifi::readWificonfigs() {
  StaticJsonDocument<200> jbuf;
  bool st = readFile(jbuf, wifi_configs);
  if (!st) {
    return false;
  }
  getWiFiValues(jbuf);
  jbuf.clear();
  Serial.println();
  Serial.println("finished reading ");
}

state_machine MyWifi::connect_wifi(bool read_cf) {

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  if (read_cf)
    readWificonfigs();

  if (wifi_data.is_set == false) {
    int i = 0;
    WiFi.begin(wifi_data.ssid, wifi_data.pass);

    while (WiFi.status() != WL_CONNECTED) {
      Serial.printf(".... %d  ", i);
      delay(500);

      if (i > this->num_trials) {
        Serial.printf("\n\rexeceed num trials restarting now ....\n\r ");
        return wifi_connect_failed;
      }
      i++;
    }
    Serial.printf("\n\rconnected \n\r ip adress %s ",
                  WiFi.localIP().toString());
    wifi_data.is_set = true;
    // wifi_data.ip_adr = WiFi.localIP().toString();
    //	d.toCharArray(wifi_data.ip_adr,d.length() );
  } else {
    Serial.println("something wrong");
    return error;
  }

  return wifi_connect_success;
}
bool MyWifi::scan_wifi(JsonDocument &jdoc) {
  // TODO: pass by refrence a json document to stor the wifi scanned data
  String ssid;
  int32_t rssi;

  WiFi.disconnect();
  int scanResult;
  // WiFi.mode(WIFI_STA);
  scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/false);
  WiFi.disconnect();
  if (scanResult == 0) {
    Serial.println(F("No networks found"));
  } else if (scanResult > 0) {
    if (scanResult > maxSSID_res)
      scanResult = maxSSID_res;
    Serial.printf("available networks %d\n", scanResult);
    for (int i = 0; i < scanResult; i++) {

      ssid = WiFi.SSID(i);
      Serial.printf("SSID: %s ,RSSI: %d dBm \n\r", ssid.c_str(), rssi);
      jdoc[i]["ssid"] = ssid;
    }
  } else {
    Serial.printf(PSTR("WiFi scan error %d"), scanResult);
  }
  return true;
}
void MyWifi::setWiFiJsondata(JsonDocument &doc) {

  doc["WiFi"] = wifi_data.ssid;
  doc["pass"] = wifi_data.pass;
  doc["IP_adress"] = wifi_data.ip_adr;
  doc["state"] = false ;//  wifi_data.is_set;
}
void MyWifi::getWiFiValues(JsonDocument &doc) {
  wifi_data.ssid = doc["WiFi"].as<String>();
  wifi_data.pass = doc["pass"].as<String>();
  wifi_data.ip_adr = doc["IP_adress"].as<String>();
  wifi_data.is_set = doc["state"];
  Serial.printf("%s %s %s %s \n", wifi_data.ssid, wifi_data.pass,
                wifi_data.ip_adr,
                wifi_data.is_set == true ? "wifi set" : " wifi not set ");
}
state_machine MyWifi::saveWiFiConfigs() {
  StaticJsonDocument<200> jbuf;
  setWiFiJsondata(jbuf);
  write_file(jbuf, wifi_configs);
  Serial.println();
  Serial.println("finished saving ");
  jbuf.clear();
  return wifi_save_success;
}
bool MyWifi::create_AP() {
  WiFi.disconnect();
  WiFi.mode(WIFI_MODE_AP);

  IPAddress subnet(255, 255, 255, 0);
  WiFi.softAPConfig(APIP, APIP, subnet);
  WiFi.softAP(ESP_ssid, NULL);
  Serial.print(" IP address = ");
  Serial.println(WiFi.softAPIP());
  return true;
}
