#include "My_ESP-NOW.h"
#include "Gateway.h"

static esp_now_peer_info peerInfo;

Device Dev_recv;
extern gateway_state gtw_state; 
const char *WIFI_SSID;
ESP_now_states ESPNOW_init(ESP_now_states st) {

  // setup wifi mode;

 //  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  if (st == ESP_NOW_Plug) {
    // get the wifi channel that the roort is connected on
    int32_t channel = getWiFiChannel(WIFI_SSID);

    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(false);
  }
  // init the ESP_NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error : failed to init ESp-NOW");
    return ESP_NOW_error;
  }
  // init peerInfo
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  // setup callback for send data
  esp_now_register_send_cb(OnDataSent);
  // setup callback for receiving data
  esp_now_register_recv_cb(OnDataReceiveCb);

  return ESP_NOW_succes;
}
int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
    for (uint8_t i = 0; i < n; i++) {
      if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
        return WiFi.channel(i);
      }
    }
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
  memcpy(&Dev_recv, incomingData, sizeof(Dev_recv));

  for (int i = 0; i < MAC_adress_size; i++) {
    printf("%02X ", mac[i]);
  }
  printf("\n\r");
  Serial.println();
  Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("device id: ");
  Serial.println(Dev_recv.mac);
  Serial.print("String  Value: ");
  Serial.println(Dev_recv.uuid);
  Serial.print("Function state" ); 
  Serial.println(Dev_recv.func.st);
  Serial.println();
  gtw_state = gateway_handle_ESP_now_recv; 
}

// callback function when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success"
                                                : "Delivery Fail");
}


void MAC_str_to_hex(String macString, MAC_adress_t &arr) {
  int startIndex = 0;
  int endIndex;
  for (int i = 0; i < 6; i++) {
    endIndex = macString.indexOf(':', startIndex);
    if (endIndex == -1) {
      endIndex = macString.length();
    }
    String hexString = macString.substring(startIndex, endIndex);
    arr[i] =
        strtol(hexString.c_str(), NULL, 16); // Convert hex string to integer
    startIndex = endIndex + 1;
  }

  // Print the array
  Serial.println("Array:");
  for (int i = 0; i < 6; i++) {
    Serial.println(arr[i], HEX);
  }
}
