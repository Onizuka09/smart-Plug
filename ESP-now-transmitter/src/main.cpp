#include "JsonFileManager.h"
#include "ACS712.h" 
#include "My_ESP-NOW.h"
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
#include "device.h"
#include "esp_now.h"
#include "pgmspace.h"
#include "RTC_module.h"
#include <cstddef>
#include <cstring>
 #include "My_mac_adr.h" 
#define pin_relay  4 
#define ACS12_pin 32 
Device dev1;
program_states prog_st;
// extern const char My_MAC_adress_str[18]; 
MAC_adress_t mac1 = {0x48, 0xE7, 0x29, 0xB4, 0x8B, 0xE0};
// {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
bool timer_st; 
MAC_adress_t mac_brod = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);
ESP_now_states ESP_now_st;
RTC_Module RTC_mod;
ACS712 ACS712_sensor(ACS712_30A, ACS12_pin);
void setup() {
  

  Serial.begin(115200);

    lcd.init();
   lcd.backlight();
    RTC_mod.RTC_Module_init(); 
    ACS712_sensor.calibrate(); 
init_littlefs();
  ESP_now_st = ESPNOW_init(specify_ESPNOW_channel);
  if (ESP_now_st != init_succes) {
    Serial.println("Error: fialed to init ESP-NOW ");
    prog_st = error_st;
  }
  addPeer(mac_brod);
  addPeer(mac1);

  memcpy(dev1.mac, My_MAC_adress_str, 18);
  memcpy(dev1.uuid, "uuvix", 5);
  dev1.func.st =  Off;
  // dev1.func.ping_st = true;

  prog_st = function_mode;
 pinMode (pin_relay,OUTPUT);
     digitalWrite(pin_relay,0); 
  }

float get_Power_data() { 
   float U = 230;

  // To measure current we need to know the frequency of current
  // By default 50Hz is used, but you can specify own, if necessary
  float I = ACS712_sensor.getCurrentAC();

  // To calculate the power we need voltage multiplied by current
  float P = U * I;
  if ( I < 1 ) { 
    I = 0; 
  } 
    Serial.print("current " ); 
    Serial.print(I);
  Serial.println(String("P = ") + P + " Watts");
  return I ;
} 
int duration = 30; 
int t= duration +1  ; 
float i ; 
void loop() {
 //              static unsigned long lastSendTime = 0;
//  unsigned long currentTime = millis();
//  if (currentTime - lastSendTime >= 16000) {
  switch (prog_st) {
  case init_st:
    // get wifi channel my mac adess and the UUID  => READ THE CREDENTIAL FILE
	dev1.func.st = On;
    prog_st = function_mode;
	 
    break;
  case function_mode:

    switch (dev1.func.st) {
    case Off:
      digitalWrite(pin_relay ,0) ;
      lcd.clear() ;
      lcd.setCursor(0,0) ; 
      lcd.print("Off mode") ; 
      Serial.println("Off function ");
      delay(2000);
      // digitalWrite(pin,LOW);
      break;
    case On:
      Serial.println("On function ");
      digitalWrite(pin_relay,1 );
      dev1.func.st = data_transmit ; 
      delay(2000);
      break;
      // digitalWrite (pin,HIGH);
    case timer:
      timer_st = RTC_mod.RTC_SetTimer_min(duration , t); 
      if (!timer_st ) { 
       dev1.func.st= Off ;  

        t = duration + 1  ;
        break; 
      } 
      digitalWrite(pin_relay, 1);
      i= get_Power_data();
      Serial.print("timer function ");
      Serial.println(t); 
      lcd.clear();
      lcd.setCursor (0,0) ; 
      lcd.print("Timer ");
      lcd.setCursor(8,0); 
      lcd.print(t);
      lcd.setCursor (12,0) ; 
      lcd.print("On ");
      lcd.setCursor(0,1 ); 
      lcd.printf("curr %.2f A", i); 
      delay(500);
      break;
	case data_transmit: 
	  // should happen each 
	  i= get_Power_data();
      Serial.print("data transmit function ");
      Serial.println(t); 
      lcd.clear();
      lcd.setCursor (0,0) ; 
      lcd.print("On mode ");
     lcd.setCursor(0,1 ); 
      lcd.printf("curr %.2f A", i); 
      delay(500);
	  break;
    case Ping:
      // handlePing();

      Serial.println("Ping Mode ");
 
      esp_err_t res = esp_now_send(mac_brod, (uint8_t *)&dev1, sizeof(dev1));

      if (res != ESP_OK)
        Serial.println("Send success");
      else
        Serial.println("Send failed");
      delay(1000);

      break;

    }

    break;
  case idle_st:
    // do nothing or waits for a callbaclk to get executed
    break;
  case error_st:

    break;
  default:
    Serial.println("Error UNkonwin state  Rebooting ...");
    delay(500);
    ESP.restart();
    break;
  }
}

/*
#include "HardwareSerial.h"
#include "My_ESP-NOW.h"
#include "WiFiGeneric.h"
#include "esp32-hal-gpio.h"
#include "esp32-hal.h"
#include "esp_err.h"
#include "http_parser.h"
#include <Arduino.h>
#include <WiFi.h>
#include <cstdint>
#include <esp_now.h>
#include <esp_wifi.h>
#define LED_pin 5
//uint8_t broadcast_adr[] ={0x30,0xC6,0xF7,0x42,0xF8,0xE4};

uint8_t broadcast_adr[] ={0x48,0xE7,0x29,0xB4,0x8B,0xE0};
// {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
typedef struct{
char b [52] ;
int id ;
float power ;
bool btn_state;
char credential[7];
}data_tx;

data_tx myData;
data_tx myDatarx;
esp_now_peer_info_t peerInfo;

void OnDataReceiveCb(const uint8_t* mac,const uint8_t* incomingData, int len){
memcpy(&myDatarx,incomingData,sizeof(myDatarx));
 Serial.print("Data received: ");
  Serial.println(len);
   Serial.print("device id: ");
  Serial.println(myDatarx.id);

  Serial.print("Led state " );
  Serial.println(myDatarx.btn_state==true ? "ON" : "OFF" );
  Serial.println();
    if (myDatarx.btn_state){
                digitalWrite(LED_pin,1);
    }
        else {
                digitalWrite(LED_pin,0);
        }
}

// callback function when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery
Fail");
}
constexpr char WIFI_SSID[] ="Galaxy A20s7654";// "NETGEAR30";

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
      for (uint8_t i=0; i<n; i++) {
          if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
              return WiFi.channel(i);
          }
      }
  }
  return 0;
}

void setup(){

Serial.begin(115200);
WiFi.mode(WIFI_STA);
//WiFi.disconnect();
int32_t channel = getWiFiChannel(WIFI_SSID);

  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  esp_wifi_set_promiscuous(false);
  WiFi.printDiag(Serial); // Uncomment to verify channel change after
if (esp_now_init() != ESP_OK){
    Serial.println("Error: failed init esp now");
        return;
}
Serial.println();

esp_now_register_send_cb(OnDataSent);

memcpy(peerInfo.peer_addr, broadcast_adr,6);

peerInfo.channel=0;
peerInfo.encrypt=false;
if (esp_now_add_peer(&peerInfo)!=ESP_OK){
    Serial.println("Failed to add peer ");
        return;
}
pinMode(LED_pin,OUTPUT);
esp_now_register_recv_cb(OnDataReceiveCb);

}

int x= 0;
void loop(){
          static unsigned long lastSendTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastSendTime >= 16000) {
        strcpy( myData.b, "hello from transmiter");
        strcpy(myData.credential , "uvw4yx") ;
myData.id=1;
x = random(1,20);
myData.power = 1.3*x;
esp_err_t res = esp_now_send(broadcast_adr,(uint8_t* )&myData,sizeof(myData));
if (res == ESP_OK)
        Serial.println("Send success");
else
        Serial.println("Send failed");

    lastSendTime = currentTime;
  }

//Serial.println("hello world !!");
}
*/
