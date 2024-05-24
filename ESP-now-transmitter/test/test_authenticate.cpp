#include <cstring>
#include <cctype>
#include <iostream>

#define mac_size_t 18

char My_MAC_adress_str[18] = "30:C6:F7:42:F8:E4";
const char My_credentials[5] = "aaaa";

typedef enum {
    Authenticate_fail = 0,
    Authenticate_success = 1,
} Authenticate_e;

Authenticate_e authenticate_device(char* mac_adr, const char* credentials) {
   char vv[mac_size_t];
    strcpy(vv, mac_adr); // Copy MAC address to a local variable

    // Convert MAC address to lowercase
    for (int i = 0; vv[i] != '\0'; i++) {
        vv[i] = toupper(vv[i]);
    }

    if (strcmp(My_MAC_adress_str, vv) != 0) {
        std::cout << "MAC not correct" << vv << std::endl;
        return Authenticate_fail;
    }
    std::cout << "MAC address correct: " << mac_adr << std::endl;
    if (0 != strcmp(My_credentials, credentials)) {
        std::cout << "credentials not correct" << std::endl;
        return Authenticate_fail;
    }

    std::cout << "credentials correct" << std::endl;
    return Authenticate_success;
}

int main() {
    Authenticate_e st;
    char vv[mac_size_t] = "30:C6:F7:42:F8:E4";
    const char cred[] = "aaaa";

    st = authenticate_device(My_MAC_adress_str, cred);

    return 0;
}

