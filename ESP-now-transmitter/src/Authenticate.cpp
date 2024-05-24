#include "Authentication.h"
#include "cstring" 
#include <cctype>
#include <stdio.h> 

#define mac_size_t 18 
const uint8_t My_MACadrss[mac_size_t] ={0x30,0xC6,0xF7,0x42,0xF8,0xE4};
// extern const char My_MAC_adress_str[18] ;

const char My_credentials[7] = "aaaa";  
Authenticate_e autheticate_device(char* mac_adr, const char* credentials) {
   char vv[mac_size_t];
    strcpy(vv, mac_adr); // Copy MAC address to a local variable

    // Convert MAC address to lowercase
    for (int i = 0; vv[i] != '\0'; i++) {
        vv[i] = toupper(vv[i]);
    }

    if (strcmp(My_MAC_adress_str, vv) != 0) {
		Serial.printf("MAc address not correct %s \n\r",vv);   
        return Authenticate_fail;
    }
   Serial.printf("MAc address  correct %s \n\r",vv);   
   /* 
    if (0 != strcmp(My_credentials, credentials)) {
        Serial.printf("Credentials NOT   correct %s \n\r",vv);   
        return Authenticate_fail;
    }

      Serial.printf("Credentials  correct %s \n\r",vv);  */
	  return Authenticate_success;
}

