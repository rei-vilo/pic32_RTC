// pic32_RTC
//
// © http://sites.google.com/site/vilorei
// CC = BY NC SA
// 
// Jun 19, 2011 - Library
//
// based on Paul_L chipKITRTCC library
//

#include "WProgram.h"
#include "pic32_RTC.h"


// Convert normal decimal numbers to binary coded decimal
uint8_t dec2bcd(uint8_t val) {
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
uint8_t bcd2dec(uint8_t val) {
  return ( (val/16*10) + (val%16) );
}

String days[7] = {
  "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


pic32_RTC::pic32_RTC() {
  _year = 0;
  _month = 0;
  _day = 0;
  _hour = 0;
  _minute = 0;
  _second = 0;

  _dayWeekNumber=0;
} 


String pic32_RTC::WhoAmI() {
  String s;
  s= "PIC32 Real Time Clock";
  return s;
}


void pic32_RTC::begin() {
  // Unlock OSCCON;
  SYSKEY = 0x0;
  SYSKEY = 0xAA996655;
  SYSKEY = 0x556699AA;
  // OSCCONbits.SOSCEN set
  OSCCONSET = 2; 
  SYSKEY = 0x0;
}


void pic32_RTC::get() {
  uint32_t rtct=RTCTIME;
  uint32_t rtcd=RTCDATE;

  rtct=rtct >>8;
  _second = bcd2dec(rtct & 0xff);
  rtct=rtct >>8;
  _minute = bcd2dec(rtct & 0xff);
  rtct=rtct >>8;
  _hour   = bcd2dec(rtct & 0xff); // 24 hours format

  _dayWeekNumber=rtcd & 0x0f;
  rtcd=rtcd >>8;
  _day = bcd2dec(rtcd & 0xff);
  rtcd=rtcd >>8;
  _month = bcd2dec(rtcd & 0xff);
  rtcd=rtcd >>8;
  _year   = bcd2dec(rtcd & 0xff); // 24 hours format
}

String pic32_RTC::time() {
  String _time = "";
  if (_hour<10) _time = _time + "0";
  _time = _time + String(_hour, 10) + ":";
  if (_minute<10) _time = _time + "0";
  _time = _time + String(_minute, 10) + ":";
  if (_second<10) _time = _time + "0";
  _time = _time + String(_second, 10);
  return _time;
}

String pic32_RTC::dayWeek() {
  // array is 0-6, but the dow register holds 1-7, so subtract 1.
  // 0 = Sunday
  return days[_dayWeekNumber - 1];    
}

String pic32_RTC::date() {
  String _date = "";
  if (_day<10) _date = _date + "0";
  _date = _date + String(_day, 10) + "/";
  if (_month<10) _date = _date + "0";
  _date = _date + String(_month, 10) + "/";
  _date = _date + String(_year, 10);
  return _date;
}

String pic32_RTC::datetime() {

  // Convenient mmddhhmm 8 characters string for file name 
  String _datetime = "";
  if (_month<10) _datetime = _datetime + "0";
  _datetime = _datetime + String(_month, 10);
  if (_day<10) _datetime = _datetime + "0";
  _datetime = _datetime + String(_day, 10);
  if (_hour<10) _datetime = _datetime + "0";
  _datetime = _datetime + String(_hour, 10);
  if (_minute<10) _datetime = _datetime + "0";
  _datetime = _datetime + String(_minute, 10);
  return _datetime;
}

uint8_t pic32_RTC::year() {
  return _year;
}

uint8_t pic32_RTC::month() {
  return bcd2dec(_month);
}

uint8_t pic32_RTC::day() {
  return _day;
}

uint8_t pic32_RTC::hour() {
  return _hour;
}

uint8_t pic32_RTC::minute() {
  return _minute;
}

uint8_t pic32_RTC::second() {
  _second;
}


void pic32_RTC::set(uint8_t year0, uint8_t month0, uint8_t day0, uint8_t hour0, uint8_t minute0, uint8_t second0)
{

  if (year0 < 100)  {
    year0 +=2000;
  }
  // Day of week calculation  
  // code by Tomohiko Sakamoto       0 = Sunday 
  // comp.lang.c on March 10th, 1993
  uint8_t y;
  uint8_t m;
  uint8_t d;
  uint8_t dow; // day of week
  static uint8_t t[] = {
    0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4                    };

  y = year0;
  m = month0;
  d = day0;

  y -= m < 3;
  dow = (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
  _dayWeekNumber=dow;

  uint32_t rtct;
  rtct=dec2bcd(hour0);
  rtct=(rtct<<8)+dec2bcd(minute0);
  rtct=(rtct<<8)+dec2bcd(second0);
  rtct=(rtct<<8);

  uint32_t rtcd;
  rtcd=dec2bcd(year0-2000);
  rtcd=(rtcd<<8)+dec2bcd(month0);
  rtcd=(rtcd<<8)+dec2bcd(day0);
  rtcd=(rtcd<<8)+dow;

  int wren = 1 ; 
  int temporar = 0 ;
  if (RTCCONbits.RTCWREN == 0 )  {
    writeEnable(); 
    wren = 0;   
  };
  RTCTIME = rtct;
  RTCDATE = rtcd;  
  if ( wren = 0) {
    writeDisable(); 
    wren = 1; 
  }

  get();
}



void pic32_RTC::writeEnable() {
  //Unlock RTCCON;
  SYSKEY = 0x0;
  SYSKEY = 0xAA996655;
  SYSKEY = 0x556699AA;
  //RTCTIMEbits.RTCWREN set
  RTCCONSET = 8; 
  SYSKEY = 0x0;
}

void pic32_RTC::writeDisable() {
  //Unlock RTCCON;
  SYSKEY = 0x0;
  SYSKEY = 0xAA996655;
  SYSKEY = 0x556699AA;
  //RTCTIMEbits.RTCWREN clear
  RTCCONCLR = 8; 
  SYSKEY = 0x0;
}



void pic32_RTC::reset() {
  if (RTCCONbits.RTCWREN == 1)   {
    RTCTIME = 0; 
  }
}





