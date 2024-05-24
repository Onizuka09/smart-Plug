#include "Mywebserver.h"
#include "ArduinoJson/Json/JsonSerializer.hpp"
#include "DNSServer.h"
#include "HardwareSerial.h"
#include "IPAddress.h"
#include "JsonFileManager.h"
#include "WString.h"
#include "WebServer.h"
#include "http_parser.h"
#include "state_machine.h"
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <LittleFS.h>
#include <WiFiClient.h>
#include <mutex>

static bool dnss_global = false;

data_s user_data;
WebServer server(80);

MyWifi wf;
state_machine prog_state = init_state;
void init_server(bool dnss) {
  // initialize wifi
  if (dnss) {
  }

  server.begin();
}

void HandelRoot() {
  File file = LittleFS.open("/index.html");
  if (!file) {
    server.send(404, "text/plain", "Can't open HTML file ");
    return;
  }
  server.streamFile(file, "text/html");
}

void Onroot() {
  server.on("/", HTTP_GET, HandelRoot);
  server.on("/generate_204", HTTP_GET, HandelRoot);
}
void OnNetworks() { server.on("/networks", HTTP_GET, HandelNetworks); }
void OnSumbmit() { server.on("/connect", HTTP_POST, HandelSumbmit); }
void OnNotFound() { server.onNotFound(handelNotFound); }

void HandelSumbmit() {
  StaticJsonDocument<200> doc;

  DeserializationError error = deserializeJson(doc, server.arg("plain"));
  if (error) {
    Serial.println("Failed to parse JSON");
    server.send(400); // Bad request
    return;
  }

  // Extract parameters from JSON
  user_data.ssid = doc["ssid"].as<String>();
  user_data.pass = doc["pass"].as<String>();
  user_data.IP = doc["serverIp"].as<String>();
  user_data.isRoot = doc["is_root"];

  Serial.println("new wifi");
  // Serial.printf("ssid %s , pass %s ,server ip  %s , %s \n", ssid, password,
  // server_IP,is_root == true ? " root selected" : "plug selected "); Connect
  // to WiFi network
  bool success = true;
  //  success = connect_wifi(ssid, password);
  // connectToWiFi(ssid, password);
  // TODO: check data format
  // update program state
  //
  //
  if (success) {
    prog_state = submit_success;
    String st;
    serializeJson(doc, st);
    Serial.println(st);
    //  save_WiFiConfig(ssid, pass);
    server.send(200, "text/plain", "data received correctly ");
    Serial.println("req finished ");

  } else {
    server.send(500, "text/plain", "Failed to connect to WiFi");
  }
}
void handelNotFound() { server.send(404, "text/plain", "Not found"); }
void HandelNetworks() {
  Serial.println("networks server requested ");
  StaticJsonDocument<200> jdoc;
  bool st = wf.scan_wifi(jdoc);

  String output;
  int error;
  if (st) {
    error = 200;
    serializeJson(jdoc, output);
  } else {
    error = 404;
    output = "Error scanning ";
  }
  Serial.println(output);
  server.send(error, "text/plain", output);
}
void handlServer() {
  server.handleClient();
  // dnsServer used to set u p a captive portal
  delay(2);
}
state_machine OTA_update() {

  WiFiClient cl;
  httpUpdate.setLedPin(2, LOW);
  
  String serverUrl ;
  if (user_data.isRoot) 
    serverUrl = "http://" + user_data.IP + ":5000/upload_root";
  else 
	serverUrl = "http://" + user_data.IP + ":5000/upload_plug";
  
  Serial.println(serverUrl);
  t_httpUpdate_return res = httpUpdate.update(cl, serverUrl);
  switch (res) {
  case HTTP_UPDATE_OK:
    Serial.println("update success");

    return upload_success;
    break;
  case HTTP_UPDATE_FAILED:
    Serial.println("Update failed ");
    break;
  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("no Update ");
    break;
  }
  return error;
}
void sendPostReq(String payload, String server_path) {
  WiFiClient cl;
  const char *host = user_data.IP.c_str();
  // String json_data =  "{\"key1\":\"value1\",\"key2\":\"value2\"}";

  const int port_s = 5000;
  //  connect to the server
  if (!cl.connect(host, port_s)) {
    Serial.println("unable to connect to  server ");
    ESP.restart();
  }
  // "/init"
  String post_req = String("POST ") + server_path + String(" HTTP/1.1\r\n");
  // SET request to POST and attach the payload
  cl.print("POST /init HTTP/1.1\r\n");
  cl.print("Host: ");
  cl.print(host);
  cl.print("\r\n");
  cl.print("Content-Type: application/json\r\n");
  cl.print("Content-Length: ");
  cl.print(payload.length());
  cl.print("\r\n\r\n");
  cl.print(payload);

  Serial.println("Request sent");
  // wait for server response
  delay(1000);
  while (cl.available()) {
    String resp = cl.readStringUntil('\r');
    Serial.println(resp);
  }
}
