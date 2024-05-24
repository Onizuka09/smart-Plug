#ifndef MYWEBSERVER_H_
#define MYWEBSERVER_H_
#include "WString.h"
#include "state_machine.h"
#include <Arduino.h>
#include <DNSServer.h>
#include <MyWiFi.h>
#include <WebServer.h>

typedef struct {
  String ssid;
  String pass;
  String IP;
  String mac_adr;
  bool isRoot;
  String My_IP; // fl chak to send or not in a separatre POST to identify my
                // self to the server

} data_s;

void handlServer();
void init_server(bool dnss);
static void enableDnss();
void Onroot();
void OnSumbmit();
void OnNetworks();
void OnNotFound();
void OnAllowed_devices() ; 
void OnFunctionOnOff(); 
void sendPostReq(String, String);
state_machine OTA_update();
// callbacks
static void HandelRoot();

static void handelNotFound();
static void HandelNetworks();
static void HandelSumbmit();

#endif
