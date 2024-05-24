//#include "ArduinoJson/Json/JsonSerializer.hpp"
#include "HardwareSerial.h"
#include "JsonFileManager.h"
#include "MyWiFi.h"
#include "Mywebserver.h"
#include "WString.h"
#include "esp32-hal-uart.h"
#include "esp32-hal.h"
#include "state_machine.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <algorithm>

DNSServer dnsServer;
MyWifi wificf;

extern data_s user_data;
extern state_machine prog_state;

void setup() {

  Serial.begin(115200);
  init_littlefs();
  bool stw = wificf.create_AP();
  IPAddress IP(172, 0, 0, 1);
  dnsServer.start(53, "*", IP);
  init_server(true);
  Onroot();
  OnNetworks();
  OnSumbmit();
  OnNotFound();
}
bool sta;
void loop() {

  handlServer();
  dnsServer.processNextRequest();
  switch (prog_state) {
  case init_state:
    break;

  case submit_success:
    Serial.println("in submit status ");
    Serial.printf("%s %s %s %d ", user_data.ssid, user_data.pass, user_data.IP,
                  user_data.isRoot);
    wificf.wifi_data.ssid = user_data.ssid;
    wificf.wifi_data.pass = user_data.pass;
    prog_state = wificf.connect_wifi(false);
    break;

  case wifi_connect_success:
	Serial.println("Saving wifi congis in LittleFS ... "); 
	wificf.wifi_data.ip_adr = user_data.IP; 
	prog_state = wificf.saveWiFiConfigs(); 
	break; 
  case wifi_save_success: 
    Serial.println("establishing an OTA update now");
    dump_file(wifi_configs); 	
	prog_state = OTA_update();
  delay(5000); 
    break;
  case upload_success:
    Serial.println("upload success rebooting now ");

    ESP.restart();
    break;
  case wifi_connect_failed:
    Serial.println("failed connecting to WiFi");
    prog_state = error;
    break;
  case error:
    Serial.println("Error state: restarting the ESP ...");
    ESP.restart();
    break;
  default:
    Serial.println("Error: default state");
    prog_state = error;
    break;
  }
}

// Serial.println("hello");
// delay(1000);
