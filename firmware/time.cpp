#include <Wire.h>
#include "time.h"
#include "timeanalog.h"
#include "touchbuttons.h"
#include "display.h"
#include "character.h"
#include "credits.h"

Chronodot RTC;

#define UPDATE_RATE_SETTING_TIME 250 
//Brett
int synchronised = 0;
int synclock = 0;


// used to show PIR ON & PIR OFF for 5 seconds
int PIRDisp = 0;
int PIROffDisp = 1;
int PIRtimer = 0;
int PIROfftimer = 0;
int PIRset = 1;
int PIROffset = 1;

//Brett
byte h, m, s; //Brett added s for seconds

byte timeMode = TIME_MODE_NORMAL;

boolean blink = true; // Used to blink the colon or the hour/minute




void timeManager(unsigned long now, boolean modeChanged)
{
  static unsigned long prev;
 
  if (timeMode == TIME_MODE_NORMAL) // Do not change mode when setting time
  {
    // Change global mode when top buttons pressed
    if (TL_KEY_DOWN)
    {
      goToPrevMode();
      return;
    }
    if (TR_KEY_DOWN)
    {
      goToNextMode();
      return;
    }
    
    // Enter setting time mode when pressing either buttons
    if ((BL_KEY_DOWN && BR_KEY_PRESSED ||
         BR_KEY_DOWN && BL_KEY_PRESSED ) &&
         displayMode == MODE_DIGITAL_CLOCK) // Allow to set time only in digital mode
    {
      timeMode = TIME_MODE_SETTING_HOUR;
      return;
    }
  }
  else //timeMode != TIME_MODE_NORMAL (Setting time)
  {
    // Exit setting time mode when pressing right bottom button
    if (BR_KEY_DOWN)
    {
      s=0;  //Brett set seconds to 0 in time setting mode BR pressed
      updateRTCtime();
      //timeMode = TIME_MODE_NORMAL;
      return;
    }
  }
  
  // Proceed only if just entered this mode or if interval has passed or if setting time
  if (!modeChanged && now-prev < TIME_INTERVAL && timeMode == TIME_MODE_NORMAL)
    return;
   //prev = now; // done below
  
  if (timeMode == TIME_MODE_NORMAL)
  {
    // Get the time from the RTC
    getRTCtime();
    //DEBUG
    /*m++;
    if (m == 60)
    {
      m=0;
      h++;
      if (h == 24)
        h=0;
    }
    */
  }
  else //timeMode != TIME_MODE_NORMAL (Setting time)
  {
    if (BL_KEY_DOWN) // Bottom left button pressed
    {
      if (timeMode == TIME_MODE_SETTING_HOUR) // Finish setting hour, switch to setting minute
      {
        timeMode = TIME_MODE_SETTING_MINUTE;
      }
      else // Finish setting minute, update RTC
      {
        updateRTCtime();
        timeMode = TIME_MODE_NORMAL;
      }  
    }
    else if (TL_KEY_DOWN) // Top left button pressed
    {
       // Decrement hour or minute
       if (timeMode == TIME_MODE_SETTING_HOUR)
       {  
          if (h == 0) h = 23;
          else h--;
       }
       else //timeMode == TIME_MODE_SETTING_MINUTE
       {
          if (m == 0) m = 59;
          else m--;
       }
       blink = true;
    }
    else if (TR_KEY_DOWN)  // Top right button pressed
    {
       // Increment hour or minute
       if (timeMode == TIME_MODE_SETTING_HOUR)
       {  
          if (h == 23) h = 0;
          else h++;
          }
       else //timeMode == TIME_MODE_SETTING_MINUTE
       {
          if (m == 59) m = 0;
          else m++;
       }
       blink = true;
    }




    //Brett
  }
   
  // Show the time on the display
  if (now-prev < UPDATE_RATE_SETTING_TIME && timeMode != TIME_MODE_NORMAL) // Hack to avoid seizure-inducing blinking when setting time
    return;
  prev = now;
  showTime();  
}

void initRTC()
{
  Wire.begin();
  RTC.begin();
  if (!RTC.isrunning())
  {
    //Serial.println(F("[WARNING] RTC is NOT running!"));
    //RTC.adjust(DateTime(__DATE__, __TIME__));
    //RTC.adjust(DateTime("Sep 12 2015", "17:00:00"));
    RTC.adjust(DateTime(0l));
  }
}

// Fill h and m variables with the current time from the RTC
void getRTCtime()
{
    DateTime now = RTC.now();  
    h = now.hour();
    m = now.minute();
    //Brett
    s = now.second();
    //Brett
}


// Update time in the RTC using h and m variables Brett added s variable. s is reset to 30 when 30 second sync pulse received
void updateRTCtime()
{
  
   synchronised = 1; // used to show sync on display
    DateTime newDateTime = DateTime(
      2015, 9, 12,
      h, m,
      s, 32, 0.0); 
    RTC.adjust(newDateTime); 
   
}




// Show the time with the currently selected clock mode
void showTime()
{
  clearFrame();
  
  switch (displayMode)
  {
    case MODE_WORD_CLOCK:
      showTimeWords();
      break;
    case MODE_DIGITAL_CLOCK:
      showTimeDigits();
      break;
    case MODE_ANALOG_CLOCK:
      showTimeAnalog();
      break;
  }
}



// Show the current time on the display using digits
void showTimeDigits()
{

  byte hTenths = h / 10; //tenth hours
  byte hUnits = h % 10; //unit
  byte mTenths = m / 10;
  byte mUnits = m % 10;
  
  // Print the hour (blink if setting hour)
  if (timeMode == TIME_MODE_NORMAL ||
      timeMode == TIME_MODE_SETTING_HOUR && blink || 
      timeMode == TIME_MODE_SETTING_MINUTE)
  {
    addCharToFrame(0           ,0            , hTenths);
    addCharToFrame(CHAR_WIDTH+1,0            , hUnits);
  }
  
  // Print the minute (blink if setting minute)
  if (timeMode == TIME_MODE_NORMAL ||
      timeMode == TIME_MODE_SETTING_HOUR ||
      timeMode == TIME_MODE_SETTING_MINUTE && blink)
  {
    addCharToFrame(0           ,CHAR_HEIGHT+1, mTenths);
    addCharToFrame(CHAR_WIDTH+1,CHAR_HEIGHT+1, mUnits);
  }
  
  // Draw blinking colon
  //frame[2][13] = blink;
  //frame[4][13] = blink;  
  setFrame(frame2,2,13,blink);
  setFrame(frame2,4,13,blink);
  blink = !blink;

  //Brett
  // Seconds Display on bottom row
    if(s == 0)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[6]);
    }

    if(s == 1)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[7]);
    }
    
    
    if(s == 2)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[8]);
    }

    if(s == 3)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[9]);
    }


    if(s == 4)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[10]);
    }


    if(s == 5)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[11]);
    }


     if(s == 6)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[12]);
    }

    if(s == 7)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[13]);
    }

    if(s == 8)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[14]);
    }


    if(s == 9)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[15]);
    }



    //------------------------------

    
    if(s == 10)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[6]);
    }

    if(s == 11)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[7]);
    }
    
    
    if(s == 12)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[8]);
    }

    if(s == 13)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[9]);
    }


    if(s == 14)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[10]);
    }


    if(s == 15)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[11]);
    }


     if(s == 16)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[12]);
    }

    if(s == 17)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[13]);
    }

    if(s == 18)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[14]);
    }


    if(s == 19)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[15]);
    }

     //------------------------------

    
    if(s == 20)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[6]);
    }

    if(s == 21)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[7]);
    }
    
    
    if(s == 22)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[8]);
    }

    if(s == 23)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[9]);
    }


    if(s == 24)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[10]);
    }


    if(s == 25)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[11]);
    }


     if(s == 26)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[12]);
    }

    if(s == 27)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[13]);
    }

    if(s == 28)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[14]);
    }


    if(s == 29)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[15]);
    }


     //------------------------------

    
    if(s == 30)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[6]);
    }

    if(s == 31)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[7]);
    }
    
    
    if(s == 32)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[8]);
    }

    if(s == 33)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[9]);
    }


    if(s == 34)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[10]);
    }


    if(s == 35)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[11]);
    }


     if(s == 36)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[12]);
    }

    if(s == 37)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[13]);
    }

    if(s == 38)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[14]);
    }


    if(s == 39)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[15]);
    }



      //------------------------------

    
    if(s == 40)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[6]);
    }

    if(s == 41)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[7]);
    }
    
    
    if(s == 42)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[8]);
    }

    if(s == 43)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[9]);
    }


    if(s == 44)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[10]);
    }


    if(s == 45)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[11]);
    }


     if(s == 46)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[12]);
    }

    if(s == 47)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[13]);
    }

    if(s == 48)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[14]);
    }


    if(s == 49)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[15]);
    }
    

      //------------------------------

    
    if(s == 50)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[6]);
    }

    if(s == 51)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[7]);
    }
    
    
    if(s == 52)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[8]);
    }

    if(s == 53)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[9]);
    }


    if(s == 54)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[10]);
    }


    if(s == 55)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[11]);
    }


     if(s == 56)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[12]);
    }

    if(s == 57)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[13]);
    }

    if(s == 58)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[14]);
    }


    if(s == 59)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[15]);
    }
    //Brett

  updateDisplay();
}

void displayPIRon() // Turns on PIR On on display
{
  PIRDisp = 1;
}


void displayPIRoff() // Turns on PIR Off on display
{
  PIROffDisp = 1;
}


// ENGLISH VERSION


// Show the current time on the display using words
void showTimeWords()
{
  byte h2 = h;

  //Brett

 /* 
 // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);

*/

  
  // Minutes
  if (m == 0) {
    
    if (h == 0) {
      addWordToFrame(w_hours[11]);
      addWordToFrame(w_oclock);
      addWordToFrame(w_at); // Brett added "at" at midnight
      addWordToFrame(w_night_ampm);// Brett added "night" at midnight
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
      
    //  addWordToFrame(w_morning_ampm);// Brett removed so morning is not shown at mid night
      
    } else if (h == 12) {
       addWordToFrame(w_hours[11]);
      addWordToFrame(w_oclock);
      addWordToFrame(w_in);// Brett added "in" at midday
      addWordToFrame(w_the_ampm);// Brett added "the" at midday
      addWordToFrame(w_afternoon_ampm);// Brett added "afternoon" at midday
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
     // addWordToFrame(w_night_ampm); // Brett removed so night is not shown at mid day
    } else {
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
      addWordToFrame(w_oclock);
    }

  } else {
  
    if (m <= 20) {
      addWordToFrame(w_minutes[m - 1]);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    } else if (m < 30) {
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
      addWordToFrame(w_minutes[19]); // twenty
      addWordToFrame(w_minutes[m - 21]);
    } else if (m == 30) {
      addWordToFrame(w_half);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    } else if (m < 40) {
      addWordToFrame(w_minutes[19]); // twenty
      addWordToFrame(w_minutes[60 - m - 21]);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    } else {
      addWordToFrame(w_minutes[60 - m - 1]);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    }
 
    if(m <= 30) {
      addWordToFrame(w_past);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    } else {
      addWordToFrame(w_to);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
      ++h2;
    }
    
  } 
  
  if(!(m ==0 && (h == 0 || h == 12))) {
  
    // Hours
    if(h2 == 0) {
      addWordToFrame(w_hours[11]);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    } else if (h2 <= 12) {
      addWordToFrame(w_hours[h2 - 1]);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    } else {
      addWordToFrame(w_hours[h2 - 13]);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    }
    /*
    if(h2 == 11 || h2 == 23) {
      addWordToFrame(w_el);
    }
    */


   // Minute & Minutes add Minutes after the minute number

   if( (m == 1) || (m ==59) )
   {
     addWordToFrame(w_minute);
     // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
   }


  // No mintues name display 5,10,15 etc mins past or to the hour
   if ( (m > 1 && m < 5) || (m > 5 && m < 10) || (m > 10 && m < 15) || (m > 15 && m < 20 ) || (m > 20 && m  <25) || (m > 25 && m < 30) || (m > 30 && m < 35) ||  (m > 35 && m < 40) || (m > 40 && m < 45  )|| (m > 45 && m < 50) || (m > 50 && m < 55) || (m > 55 && m < 59))
   {
    addWordToFrame(w_minute);
    addWordToFrame(w_s);
   }
   
    // Time of day
   // if(h < 6) {
      if(h < 1) { // Brett changed so clock says Morning after midnight
      addWordToFrame(w_at);
      addWordToFrame(w_night_ampm);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    } else if(h < 12) {
      addWordToFrame(w_in);
      addWordToFrame(w_the_ampm);
      addWordToFrame(w_morning_ampm);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    } else if(h < 18) {
      addWordToFrame(w_in);
      addWordToFrame(w_the_ampm);
      addWordToFrame(w_afternoon_ampm);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    } else if(h >= 18 &&  h < 21) {
      addWordToFrame(w_in);
      addWordToFrame(w_the_ampm);
      addWordToFrame(w_evening_ampm);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    } else {
      addWordToFrame(w_at);
      addWordToFrame(w_night_ampm);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    }


    // Minutes Quarter add "a" to quarter past and quarter to
    if (m == 15 or m == 45)
    {
      addWordToFrame(w_a);
      // Show "THE TIME IS"
  addWordToFrame(w_the);
  addWordToFrame(w_time);
  addWordToFrame(w_is);
    }


//show "(SECONDS) SYNC" on display whenever a sync pulse arrives
// seconds are reset to 31 sync is displayed untill they reach 36 then go off
  if ((s >= 1 && s <= 59) && synchronised == 1) 
    {
    if ( s <= 30 )
    {
     
      addWordToFrame(w_sync_sync);
    } 
    }
  
    else 
    {
    synchronised = 0; 
  
    }


    
  }
  
// Seconds Display on bottom row
    if(s == 0)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[6]);
    }

    if(s == 1)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[7]);
    }
    
    
    if(s == 2)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[8]);
    }

    if(s == 3)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[9]);
    }


    if(s == 4)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[10]);
    }


    if(s == 5)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[11]);
    }


     if(s == 6)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[12]);
    }

    if(s == 7)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[13]);
    }

    if(s == 8)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[14]);
    }


    if(s == 9)
    {
      addWordToFrame(w_seconds[0]);
      addWordToFrame(w_seconds[15]);
    }



    //------------------------------

    
    if(s == 10)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[6]);
    }

    if(s == 11)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[7]);
    }
    
    
    if(s == 12)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[8]);
    }

    if(s == 13)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[9]);
    }


    if(s == 14)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[10]);
    }


    if(s == 15)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[11]);
    }


     if(s == 16)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[12]);
    }

    if(s == 17)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[13]);
    }

    if(s == 18)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[14]);
    }


    if(s == 19)
    {
      addWordToFrame(w_seconds[1]);
      addWordToFrame(w_seconds[15]);
    }

     //------------------------------

    
    if(s == 20)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[6]);
    }

    if(s == 21)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[7]);
    }
    
    
    if(s == 22)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[8]);
    }

    if(s == 23)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[9]);
    }


    if(s == 24)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[10]);
    }


    if(s == 25)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[11]);
    }


     if(s == 26)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[12]);
    }

    if(s == 27)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[13]);
    }

    if(s == 28)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[14]);
    }


    if(s == 29)
    {
      addWordToFrame(w_seconds[2]);
      addWordToFrame(w_seconds[15]);
    }


     //------------------------------

    
    if(s == 30)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[6]);
    }

    if(s == 31)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[7]);
    }
    
    
    if(s == 32)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[8]);
    }

    if(s == 33)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[9]);
    }


    if(s == 34)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[10]);
    }


    if(s == 35)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[11]);
    }


     if(s == 36)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[12]);
    }

    if(s == 37)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[13]);
    }

    if(s == 38)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[14]);
    }


    if(s == 39)
    {
      addWordToFrame(w_seconds[3]);
      addWordToFrame(w_seconds[15]);
    }



      //------------------------------

    
    if(s == 40)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[6]);
    }

    if(s == 41)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[7]);
    }
    
    
    if(s == 42)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[8]);
    }

    if(s == 43)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[9]);
    }


    if(s == 44)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[10]);
    }


    if(s == 45)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[11]);
    }


     if(s == 46)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[12]);
    }

    if(s == 47)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[13]);
    }

    if(s == 48)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[14]);
    }


    if(s == 49)
    {
      addWordToFrame(w_seconds[4]);
      addWordToFrame(w_seconds[15]);
    }
    

      //------------------------------

    
    if(s == 50)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[6]);
    }

    if(s == 51)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[7]);
    }
    
    
    if(s == 52)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[8]);
    }

    if(s == 53)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[9]);
    }


    if(s == 54)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[10]);
    }


    if(s == 55)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[11]);
    }


     if(s == 56)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[12]);
    }

    if(s == 57)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[13]);
    }

    if(s == 58)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[14]);
    }


    if(s == 59)
    {
      addWordToFrame(w_seconds[5]);
      addWordToFrame(w_seconds[15]);
    }
    
   // PIR ON Display

  // Show PIR ON for 5 seconds when activated by Touch button when in the Word Clock
  
  if( PIRDisp == 1 && PIRset ==1 && PIROfftimer == 0 )  // PIROfftimer == 0 stops PIR ON showing at the same time as PIR OFF. Note PIR will still activate
  
  {
   PIRtimer = s + 5; // set time PIR ON is displayed
   PIRset = 0;
    
  }
  
  
  if(  s < PIRtimer && s != 0  ) // && s != 0 turns off PIR ON at 0 seconds in case PIR turned on after 54 seconds
  
  {
    
  addWordToFrame(w_pir);
  addWordToFrame(w_on);
}

 else
 {
  PIRtimer = 0;
  PIRset = 1;
  PIRDisp = 0; // stops PIR repeatedly displaying
 }

 

 // PIR OFF display
 // Show PIR OFF for 5 seconds when deactivated by Touch button when in the Word Clock
 
  if( PIROffDisp == 1 && PIROffset ==1 && PIRtimer == 0 ) // PIRtimer == 0 stops PIR OFF showing at the same time as PIR ON. Note PIR will still deactivate
  
  {
   PIROfftimer = s + 5; // set time PIR OFF is displayed
   PIROffset = 0;
    
  }
  
  
  if(  s < PIROfftimer && s != 0 )
  
  {
    
  addWordToFrame(w_pir);
  addWordToFrame(w_off);
}

 else
 {
  PIROfftimer = 0;
  PIROffset = 1;
  PIROffDisp = 0; // stops PIR repeatedly displaying
 } 
  


 
  updateDisplay();
}
/*
void afegirDpelsQuarts(byte h2)
{
  if      (h2 ==  1 || h2 == 13)
    addWordToFrame(w_h_d_apost_u);
  else if (h2 == 11 || h2 == 23)
    addWordToFrame(w_h_d_apost_o);
  else
    addWordToFrame(w_h_de);
}

void afegirLaLesHores(byte hora) 
{
  if (hora == 1 || hora == 13)
    addWordToFrame(w_la);
  else
    addWordToFrame(w_les);
}

void afegirEsSon(byte hora, byte minut)
{
  if (hora == 1 || hora == 13) {
    if (minut >= 0 && minut <= 7 || minut >= 15 && minut <= 22) //It is between one and a half fourth. It is between a quarter and a quarter and a half
      addWordToFrame(w_es);
    else                            //They are two quarters / They are two quarters and a half / They are three quarters / They are three quarters and a half
      addWordToFrame(w_son);
  } 
  else 
  {
    if (minut == 7 || minut >= 15 && minut <= 22)       // És mig quart / És entre un quart i  un quart i mig
      addWordToFrame(w_es);
    else
      addWordToFrame(w_son);                  //Són les dues en punt / Són dos quarts / Són dos quarts i mig / Són tres quarts / Són tres quarts i mig
  }
}
*/
// END ENGLISH VERSION

