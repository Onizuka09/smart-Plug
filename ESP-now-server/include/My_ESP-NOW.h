#ifndef My_ESP_NOW_H_
#define My_ESP_NOW_H_
#include "HardwareSerial.h"
#include "device.h"

#include <Arduino.h>
#include <WiFi.h>
#include <cstdint>
#include <esp_now.h>
#include <esp_wifi.h>

#define MAC_adress_size 6

typedef uint8_t MAC_adress_t[MAC_adress_size];
typedef enum {
  ESPNOW_gateway = 0,
  ESP_NOW_Plug,
  ESP_NOW_succes,
  ESP_NOW_error,

} ESP_now_states;

ESP_now_states ESPNOW_init(ESP_now_states st);

void addPeer(MAC_adress_t madr);

void OnDataReceiveCb(const uint8_t *mac, const uint8_t *incomingData, int len);
// callback function when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
int32_t getWiFiChannel(const char *ssid);
void MAC_str_to_hex(String macString, MAC_adress_t &arr) ;

#endif
