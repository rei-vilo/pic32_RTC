// pic32_RTC
//
// © http://sites.google.com/site/vilorei
// CC = BY NC SA
// 
// Jun 19, 2011 - Library
//
// based on Paul_L chipKITRTCC library
//


#ifndef pic32_RTC_h
#define pic32_RTC_h

#include "WProgram.h"

class pic32_RTC
{
public:
  pic32_RTC(); // constructor
  String WhoAmI();
  void begin();
  void get();
  void set(uint8_t year0, uint8_t month0, uint8_t day0, uint8_t hour0, uint8_t minute0, uint8_t second0);
  void reset();
  
  uint8_t year();
  uint8_t month();
  uint8_t day();
  uint8_t hour();
  uint8_t minute();
  uint8_t second();

  String dayWeek();
  String date();
  String time();
  String datetime();

private:
  uint8_t _dayWeekNumber;
  uint8_t _year;
  uint8_t _month;
  uint8_t _day;
  uint8_t _hour;
  uint8_t _minute;
  uint8_t _second;

void writeEnable();
void writeDisable();

};

#endif





