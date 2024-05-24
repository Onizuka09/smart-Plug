#ifndef GATEWAY_H_ 
#define GATEWAY_H_ 
#include <Arduino.h>
 #include "device.h"
 
#include "My_ESP-NOW.h" 
#define max_allowed_devcices 4 

typedef enum {
Init_gateway=0, 
gateway_handle_allowed_devices,
gateway_handle_ESP_now_recv, 
gateway_Handle_functions, 
gateway_handle_ON, 
gateway_handle_off, 
gateway_Handle_timer, 
gateway_error_state,

}gateway_state; 

void set_function(function_e st);

typedef enum {
  Dev1_e= 0, 
  Dev2_e, 
  Dev3_e, 
  Dev4_e, 
  Dev_notFound 
}My_devcices_e;
My_devcices_e Is_device_allowed(char* mac, Device (&allowed_devcies)[max_allowed_devcices], uint8_t len) ; 
#endif 
