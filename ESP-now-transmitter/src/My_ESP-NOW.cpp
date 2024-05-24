#include "My_ESP-NOW.h"
#include "HardwareSerial.h"
#include "device.h"
#include "Authentication.h" 
#include <cstring>
// #include "My_mac_adr.h" 
#include "JsonFileManager.h" 
static esp_now_peer_info peerInfo;
extern const char My_MAC_adress_str[18]; //  = "31:C8:47:62:28:14"; //"30:C6:F7:42:F8:E4";
Device check_dev;
extern Device dev1;

const char *WIFI_SSID;
ESP_now_states ESPNOW_init(ESP_now_states st) {

  // setup wifi mode;

  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  if (st == specify_ESPNOW_channel) {
    // get the wifi channel that the roort is connected on
    int32_t channel = getWiFiChannel();

    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(false);
  }
  // init the ESP_NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error : failed to init ESp-NOW");
    return error;
  }
  // init peerInfo
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // setup callback for send data
  esp_now_register_send_cb(OnDataSent);
  // setup callback for receiving data
  esp_now_register_recv_cb(OnDataReceiveCb);

  return init_succes;
}
int32_t getWiFiChannel() {
 StaticJsonDocument<200> jbuf;
  bool st = readFile(jbuf, wifi_configs);
  Serial.print("File read comelete  SSID:   ") ; 
  String ssid = jbuf["WiFi"].as<String>();

  Serial.println(ssid);
 //  ssid.toCharArray(char *buf, unsigned int bufsize)

  if (int32_t n = WiFi.scanNetworks()) {
    for (uint8_t i = 0; i < n; i++) {
      if (!strcmp(ssid.c_str(), WiFi.SSID(i).c_str())) {
        Serial.println("channel Found " );
        return WiFi.channel(i);
      }
    }
    Serial.println("Channel was not found ... Wrong SSID "); 
  }
  return 0;
}
void addPeer(MAC_adress_t madr) {

  if (!esp_now_is_peer_exist(madr)) {

    memcpy(&peerInfo.peer_addr, madr, MAC_adress_size);
    esp_now_add_peer(&peerInfo);
  } else {
    Serial.println("Peer already exist");
  }
}
void removePeer(MAC_adress_t madr) {
  if (esp_now_del_peer(madr) != ESP_OK) {

    Serial.println("Error: couldn't remove peer");
  } else {
    Serial.println("Peer removed");
  }
}
void OnDataReceiveCb(const uint8_t *mac, const uint8_t *incomingData, int len) {
  Serial.println("aaaa");
  memcpy(&check_dev, incomingData, sizeof(check_dev));
    if ( autheticate_device(check_dev.mac, check_dev.uuid) == Authenticate_fail){
		Serial.println("Error Authentication failed" ); 
		return;
    }
	Serial.println("Authentication success" ); 
	memcpy(&dev1, incomingData,sizeof(dev1));
  /* memcpy(&myDatarx,incomingData,sizeof(myDatarx));
   Serial.print("Data received: ");
    Serial.println(len);
     Serial.print("device id: ");
    Serial.println(myDatarx.id);
    // here i set flag of wich function to be executed - the func implementation
   is in the main Serial.print("Led state " );
    Serial.println(myDatarx.btn_state==true ? "ON" : "OFF" );
    Serial.println();
      if (myDatarx.btn_state){
                  digitalWrite(LED_pin,1);
      }
          else {
                  digitalWrite(LED_pin,0);
          }*/
}

// callback function when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success"
                                                : "Delivery Fail");
}
