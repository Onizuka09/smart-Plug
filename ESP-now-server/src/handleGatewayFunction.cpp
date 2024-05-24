#include "Gateway.h"
#include "device.h"
#include <cstdint>
#include <sys/types.h>
#define MAC_STR_len 18 
extern Device dev1 ; 
static void handleFuncOn();
static void handleFuncOff();
static void handlePing();
static void handleTimer();
static MAC_adress_t mac_brod = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
void set_function(function_e st) {
  switch (st) {
  case Off:
    handleFuncOff();
    // digitalWrite(pin,LOW);
    break;
  case On:
    handleFuncOn();
    // digitalWrite (pin,HIGH);
  case timer:
    handleTimer();
    // Access the timer union member and perform actions
    // For example:
    // myDevice.timer.min = 0;
    // myDevice.timer.hour = 12;
    // myDevice.timer.day = 1;
    break;
  case Ping:
    handlePing();
    // Handle ping function
    break;
  default:

    // Handle error
    break;
  }
}
 void handleFuncOn() {
  // set payload  set the MAC adress and the UUID first 
  // Set dev function 
   Serial.println("HAdnle on ");
    dev1.func.st = data_transmit;
    esp_err_t res = esp_now_send(mac_brod, (uint8_t *)&dev1, sizeof(dev1));

    if (res != ESP_OK)
      Serial.println("Send success");
    else
      Serial.println("Send failed");
    delay(1000);
  }


void handleFuncOff() { Serial.println("HAdnle off  "); }
void handlePing() { Serial.println("HAdnle ping  "); }
void handleTimer() { Serial.println("HAdnle timer  "); }
My_devcices_e Is_device_allowed(char* mac, Device (&allowed_devcies)[max_allowed_devcices], uint8_t len)  {
for ( int i= 0; i< len; ++i ) {
    if ( 0 == strcmp(mac, allowed_devcies[i].mac ) ) { 
      My_devcices_e dv =  static_cast<My_devcices_e>(i);
        Serial.printf ("Found devcie at %d: %s  \n\r", dv , mac); 
      return dv ; 
}


} 
return Dev_notFound; 
}

