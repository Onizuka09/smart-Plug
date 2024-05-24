#include "HardwareSerial.h"
#include "Gateway.h"
#include "JsonFileManager.h"
#include "My_ESP-NOW.h"
#include "device.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include <Arduino.h>
#include <WiFi.h>
#include <cstdint>
#include <cstring>
#include <esp_now.h>
#include "MyWiFi.h"
#include "state_machine.h"
#include "Mywebserver.h" 
gateway_state gtw_state = Init_gateway;
extern Device Dev_recv ; 
MAC_adress_t mac = { 0x30,0xC6, 0xF7, 0x42,0xF8, 0xE4} ;

MAC_adress_t mac_brod = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
esp_err_t res ;
My_devcices_e d; 
 Device dev1;
Device dev2 ; 
Device dev3 ; 
Device allowed_devcies[max_allowed_devcices]; 
ESP_now_states ESP_now_st;
MyWifi wificf; 

void setup() {
  Serial.begin(115200);
  init_littlefs();

  if ( wificf.connect_wifi(true) != wifi_connect_success) {
    Serial.println("fialed to connect to wifi " ); 
     gtw_state = gateway_error_state;
    return ; 
  }
 init_server(false);
 OnAllowed_devices() ; 
 OnFunctionOnOff(); 
  OnNotFound();
      ESP_now_st = ESPNOW_init(ESPNOW_gateway);
  if (ESP_now_st !=  ESP_NOW_succes) {
    Serial.println("Error: fialed to init ESP-NOW ");
    return;
  }
  addPeer(mac_brod);
  // addPeer(mac);

char My_MAC_adress_str[18] = "30:C6:F7:42:F8:E4";
const char My_credentials[5] = "aaaa";

strcpy ( dev1.mac ,  My_MAC_adress_str ) ; 
strcpy(dev1.uuid ,  My_credentials)  ; 
dev1.func.st = On; 
allowed_devcies[Dev1_e] = dev1 ; 

strcpy ( dev2.mac , "31:C8:47:62:28:14") ; 
strcpy(dev2.uuid , "bbbb"); 
dev2.func.st = Off; 
allowed_devcies[Dev2_e] = dev2 ;
 for (int i = 0; i < 3; ++i) {
    Serial.print("Device ");
    Serial.print(i);
    Serial.print(" MAC: ");
    Serial.print(allowed_devcies[i].mac);
    Serial.print(", UUID: ");
    Serial.print(allowed_devcies[i].uuid);
    Serial.print(", Function state: ");
    Serial.println(allowed_devcies[i].func.st);
    Serial.println("----------------------" ); 
  }
 
}

void loop() {
  handlServer();

switch (gtw_state) {
    case Init_gateway:
        // Code for handling Init_gateway state
      // Serial.println("init- "); 
        break;
    case gateway_handle_allowed_devices:
        // Code for handling gateway_handle_allowed_devices state
        break;
     case gateway_Handle_functions:

        // Code for handling gateway_Handle_functions state
     /*    allowed_devcies[1].func.st= Off; 
      res = esp_now_send(mac_brod, (uint8_t *)&allowed_devcies[1], sizeof(allowed_devcies[1]));
    if (res != ESP_OK)
      Serial.println("Send success");
    else
      Serial.println("Send failed");
    delay(1000);  
    gtw_state = Init_gateway ;
    break;    
    case gateway_handle_ESP_now_recv:
       if ( Dev_recv.func.st == Ping) {
             d  = Is_device_allowed(Dev_recv.mac, allowed_devcies,max_allowed_devcices) ; 
            if ( d == Dev_notFound) {

              Serial.println("Unkown Device" );
            }else {
            Serial.println("Device found ... to beconfigure ..." );

                            
            gtw_state = gateway_Handle_functions;
            }
        }
        else { 
              gtw_state = gateway_Handle_functions; 
               
        }*/
      
        // Code for handling gateway_Handle_functions state
        break;
    case gateway_handle_ON: 
       Serial.println("ON " ); 
        allowed_devcies[1].func.st=On; 
           res = esp_now_send(mac_brod, (uint8_t *)&allowed_devcies[1], sizeof(allowed_devcies[d]));
    if (res != ESP_OK)
      Serial.println("Send success");
    else
      Serial.println("Send failed");
    delay(1000);  
    gtw_state = Init_gateway ;
        break; 

    case gateway_handle_off: 
         allowed_devcies[1].func.st= Off; 
           res = esp_now_send(mac_brod, (uint8_t *)&allowed_devcies[1], sizeof(allowed_devcies[d]));
    if (res != ESP_OK)
      Serial.println("Send success");
    else
      Serial.println("Send failed");
    delay(1000);  
    gtw_state = Init_gateway ;

        Serial.println("OFF ");

        break; 
    case gateway_Handle_timer: 
 Serial.println("Timer " ); 
        allowed_devcies[1].func.st=timer; 
        // allowed_devcies[1].min
           res = esp_now_send(mac_brod, (uint8_t *)&allowed_devcies[1], sizeof(allowed_devcies[d]));
    if (res != ESP_OK)
      Serial.println("Send success");
    else
      Serial.println("Send failed");
    delay(1000);  
    gtw_state = Init_gateway ;
        break; 



 
    case gateway_error_state:
        Serial.println("Error: fatal error restarting ESP ... "); 
        delay(500); 
        ESP.restart(); 

        // Code for handling gateway_error_state state
        break;
    default:
        // Code for handling unknown state
        break;
}


 }

// Serial.println("hello world !!");
