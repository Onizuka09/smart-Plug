// Online C++ compiler to run C++ program online
#include <iostream>
#include <stdio.h>
#include <string> 
using namespace std ;
#define MAC_adress_size 6

typedef uint8_t MAC_adress_t[MAC_adress_size] ;
void test (){
string macString = "33:44:45:49:24:27";
  MAC_adress_t mac1; 

  int startIndex = 0;
  int endIndex;
  for (int i = 0; i < 6; i++) {
      endIndex = macString.find(":",startIndex);
      //.(':', startIndex);
      if (endIndex == -1)
      {
          endIndex = macString.length();
    }
    string hexString = macString.substr(startIndex, endIndex);
    mac1[i] = strtol(hexString.c_str(), NULL, 16); // Convert hex string to integer
    startIndex = endIndex + 1;
  }

  // Print the array
  printf("Array:");
  for (int i = 0; i < 6; i++) {
    printf("%02X ",mac1[i]);
  }
  printf("\n");
}
int main() {
    // Write C++ code here
    cout << "Try programiz.pro\n";
    test(); 
    return 0;
}