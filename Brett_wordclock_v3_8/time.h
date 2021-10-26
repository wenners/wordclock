#ifndef TIME_H
#define TIME_H

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include "Chronodot.h"  // Real time clock library    - https://github.com/Stephanie-Maks/Arduino-Chronodot

// The real time clock chip (DS3231)
extern Chronodot RTC;

#define TIME_MODE_NORMAL         0
#define TIME_MODE_SETTING_HOUR   1
#define TIME_MODE_SETTING_MINUTE 2


// Clock variables
extern byte h, m, s; //Brett added s

#define TIME_INTERVAL 1000 // Update time every second, this way the colon will blink and the numbers will change at minute changes





void displayPIRon(); // Show PIR On  on the display
void displayPIRoff(); //Show PIR  Off on the display

void timeManager(unsigned long now, boolean modeChanged);

// Initializes the RTC
void initRTC();

// Fill h and m variables with the current time from the RTC
void getRTCtime();

// Update time in the RTC using h and m variables
void updateRTCtime();

// Show the time with the currently selected clock mode
void showTime();

// Show the current time on the display using digits
void showTimeDigits();








//ENGLISH
// Show the current time on the display using words
void showTimeWords();

// Words
// Format: { line index, start position index, length }
const byte              w_the[3] PROGMEM = { 0,  0, 3 };
const byte             w_time[3] PROGMEM = { 0,  4, 4 };    
const byte               w_is[3] PROGMEM = { 0,  9, 2 };
const byte             w_half[3] PROGMEM = { 0, 12, 4 };

const byte                w_a[3] PROGMEM = { 0, 13, 1 };
      
const byte           w_minute[3] PROGMEM = { 7, 0, 6 };
const byte                w_s[3] PROGMEM = { 7, 6, 1 };
const byte             w_past[3] PROGMEM = { 7, 8, 4 };
const byte               w_to[3] PROGMEM = { 7,11, 2 };

const byte           w_oclock[3] PROGMEM = { 11, 0, 6 };
const byte               w_in[3] PROGMEM = { 11, 7, 2 };
const byte               w_at[3] PROGMEM = { 11, 9, 2 };


const byte           w_night_ampm[3] PROGMEM = { 12, 0, 5 };
const byte             w_the_ampm[3] PROGMEM = { 12, 4, 3 };
const byte         w_morning_ampm[3] PROGMEM = { 12, 8, 7 };



const byte         w_evening_ampm[3] PROGMEM = { 13, 0, 7 };
const byte       w_afternoon_ampm[3] PROGMEM = { 13, 7, 9 };
const byte            w_noon_ampm[3] PROGMEM = { 13,12, 4 };


//const byte         w_seconds_sync[3] PROGMEM = { 14, 0, 7 };
//const byte              w_in_sync[3] PROGMEM = { 14, 8, 2 };
const byte                    w_pir[3] PROGMEM = { 14,0, 3 };
const byte                     w_on[3] PROGMEM = { 14,4, 2 };
const byte                    w_off[3] PROGMEM = { 14,7, 3 };
const byte              w_sync_sync[3] PROGMEM = { 14,12, 4 };

      
const byte w_minutes[20][3] PROGMEM = {  // [No of items from 0] 
  { 2, 12, 3 }, // one
  { 2, 10, 3 }, // two
  { 6, 11, 5 }, // three
  { 5,  0, 4 }, // four
  { 3,  8, 4 }, // five
  { 2,  3, 3 }, // six
  { 4,  0, 5 }, // seven
  { 3,  0, 5 }, // eight
  { 4,  8, 4 }, // nine
  { 2,  0, 3 }, // ten
  { 6,  5, 6 }, // eleven
  { 6,  0, 6 }, // twelve
  { 5,  8, 8 }, // thirteen
  { 5,  0, 8 }, // fourteen
  { 1,  0, 7 }, // quarter
  { 2,  3, 7 }, // sixteen
  { 4,  0, 9 }, // seventeen
  { 3,  0, 8 }, // eighteen
  { 4,  8, 8 }, // nineteen
  { 1,  7, 6 }  // twenty
};

const byte w_hours[12][3] PROGMEM = {
  { 8,  2, 3 }, // one
  { 8,  0, 3 }, // two
  { 9,  5, 5 }, // three
  { 10, 0, 4 }, // four
  { 10, 5, 4 }, // five
  { 8, 13, 3 }, // six
  { 9,  0, 5 }, // seven
  { 10, 8, 5 }, // eight
  { 8,  9, 4 }, // nine
  { 10, 12, 3 }, // ten
  { 8,  4, 6 }, // eleven
  { 9, 10, 6 }  // twelve
};



const byte w_seconds[16][3] PROGMEM = {
  { 15,  0, 1 }, // t0  t is tens of seconds 
  { 15,  1, 1 }, // t1
  { 15,  2, 1 }, // t2
  { 15,  3, 1 }, // t3
  { 15,  4, 1 }, // t4
  { 15,  5, 1 }, // t5
  { 15,  6, 1 }, // u0 u is units of seconds
  { 15,  7, 1 }, // u1
  { 15,  8, 1 }, // u2
  { 15,  9, 1 }, // u3
  { 15, 10, 1 }, // u4
  { 15, 11, 1 }, // u5
  { 15, 12, 1 }, // u6
  { 15, 13, 1 }, // u7
  { 15, 14, 1 }, // u8
  { 15, 15, 1 }, // u9
  
   
};


//End ENGLISH


#endif


