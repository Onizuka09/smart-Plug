#ifndef RTC_MOD
#define RTC_MOD 
//  #include "RtcDateTime.h"
#include <RtcDS1302.h>
#include <ThreeWire.h>
#include <Arduino.h>


class RTC_Module {
private:
  RtcDateTime start_time;
  uint32_t duration_seconds;
  bool time_func; 
    bool end_timer = false ;
public:

  int start_min; 
int prev_min ;

  RTC_Module():start_min(0),prev_min(0),time_func(false){}  
 void  RTC_Timer(uint32_t seconds);  

void RTC_Module_init(); 
void printDateTime(RtcDateTime& dt); 

RtcDateTime RTC_getDateTime();
  bool isElapsed() ; 
bool RTC_SetTimer_min(int duration, int& t ) ;

}; 

#endif 
