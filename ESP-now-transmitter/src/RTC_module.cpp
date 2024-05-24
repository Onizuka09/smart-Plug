#include "RTC_module.h"
#include "HardwareSerial.h"
#include "RtcDateTime.h"

#define DAT 27 
#define CLK 14
#define RST 26 
  ThreeWire wire(DAT,CLK,RST); 
  RtcDS1302<ThreeWire> Rtc(wire); 




 void  RTC_Module::RTC_Timer(uint32_t seconds) {
    duration_seconds = seconds;
    start_time = Rtc.GetDateTime();
  }
void RTC_Module::RTC_Module_init(){
       Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid()) {
    // Common Causes:
    //    1) first time you ran and the device wasn't running yet
    //    2) the battery on the device is low or even missing

    Serial.println("RTC lost confidence in the DateTime!");
    Rtc.SetDateTime(compiled);
  }

  if (Rtc.GetIsWriteProtected()) {
    Serial.println("RTC was write protected, enabling writing now");
    Rtc.SetIsWriteProtected(false);
  }

  if (!Rtc.GetIsRunning()) {
    Serial.println("RTC was not actively running, starting now");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now = Rtc.GetDateTime();
  if (now < compiled) {
    Serial.println("RTC is older than compile time!  (Updating DateTime)");
    Rtc.SetDateTime(compiled);
  } else if (now > compiled) {
    Serial.println("RTC is newer than compile time. (this is expected)");
  } else if (now == compiled) {
    Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }

   }
/* 
RtcDateTime RTCGetDateTime() { 
  RtcDateTime dt = RTC_getDateTime(); 
	if (!dt.IsValid()){
		Serial.println("Error: Lost confidence in RTC"); 
		// create an Erro func that would notify the USER like setting an Error flag 
	}
  return dt ; 

}*/ 
void RTC_Module::printDateTime(RtcDateTime& dt) {
  char datestring[20];

  snprintf_P(datestring,
             countof(datestring),
             PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
             dt.Month(),
             dt.Day(),
             dt.Year(),
             dt.Hour(),
             dt.Minute(),
             dt.Second());
  Serial.println(datestring);
}


  bool RTC_Module::isElapsed() {
    RtcDateTime current_time = Rtc.GetDateTime();
   uint32_t elapsed_seconds ;// = current_time - start_time;
    return elapsed_seconds >= duration_seconds;
  }
  
RtcDateTime RTC_Module::RTC_getDateTime(){
	RtcDateTime now = Rtc.GetDateTime(); 
	if (!now.IsValid()){
		Serial.println("Error: Lost confidence in RTC"); 
		// create an Erro func that would notify the USER like setting an Error flag 
	}
	return now; 
}
bool  RTC_Module::RTC_SetTimer_min(int duration, int& t) {  
  // timer=true  ; 
  // int duration = 10 ; // seconds 
  RtcDateTime start = Rtc.GetDateTime(); 
  // RtcDateTime prev; 
  // setup  timer 
  static int tess =-1  ; 
  if ( tess != start_min ) { 
   t --; 
   tess = start_min ; 
  } 
  start_min = (int )start.Second();
  if ( end_timer == false) { 
  prev_min = start_min + duration; 
  if ( prev_min >= 60 ){ 
     prev_min = prev_min - 60;  
     time_func = true; 
  }
     end_timer = true ; 
  }
  if (start_min == 0 ) { 
    time_func = false ; 
  } 
 //  t= prev_min ; 
  Serial.print(".cpp " );
   Serial.println(t);
 //  Serial.println(prev_min);
 //  Serial.println(start.Second(),DEC);
  
  if ( !time_func && start_min >= prev_min ) { 
        Serial.println( " Stoping timer now" ); 
        prev_min = 0 ;
        end_timer=false ; 
        tess = -1 ; 
          return false ;
      
 
}else if ( time_func && start_min <= prev_min ) { 
        Serial.println( " Stoping timer now" ); 
        prev_min = 0 ;
        time_func =false ; 
        end_timer = false ;
        tess = -1 ;
          return false ;
      }


  return true ;
} 




