/*
#include "HardwareSerial.h"
#include "esp_err.h"
#include <Arduino.h>
#include <WiFi.h>
#include <cstdint>
#include <cstring>
#include <esp_now.h>




typedef struct{
        char a[52];
        int id ;
        float power;


}struct_message;
struct_message data_tx;

void OnDataReceiveCb(const uint8_t* mac,const uint8_t* incomingData, int len){
memcpy(&data_tx,incomingData,sizeof(data_tx));
 Serial.print("Data received: ");
  Serial.println(len);
  Serial.print("device id: ");
  Serial.println(data_tx.id);
  Serial.print("String  Value: ");
  Serial.println(data_tx.a);
  Serial.print("power Value: ");
  Serial.println(data_tx.power);

  Serial.println();

}


void setup(){

Serial.begin(115200);
WiFi.mode(WIFI_STA);

if (esp_now_init() != ESP_OK ) {
    Serial.println("Error init ESP NOW ");
    return;
}
esp_now_register_recv_cb(OnDataReceiveCb);

}

void loop(){

//Serial.println("hello world !!");
}
*/
