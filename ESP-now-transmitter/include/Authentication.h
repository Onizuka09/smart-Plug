#ifndef  AUTHENTICATION_H_
#define AUTHENTICATION_H_ 
#include <Arduino.h>
#include "My_mac_adr.h"

typedef enum{
Authenticate_fail=0, 
Authenticate_success=1, 
}Authenticate_e; 


Authenticate_e autheticate_device ( char* mac_adr, const char* credentials); 



#endif 
