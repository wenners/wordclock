#include <Wire.h>
#include "time.h"
#include "timeanalog.h"
#include "touchbuttons.h"
#include "display.h"
#include "character.h"
#include "credits.h"

Chronodot RTC;

#define UPDATE_RATE_SETTING_TIME 250 

byte h, m;
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
    
    // Enter setting time mode when pressing both bottom buttons
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
      updateRTCtime();
      timeMode = TIME_MODE_NORMAL;
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
}


// Update time in the RTC using h and m variables
void updateRTCtime()
{
    DateTime newDateTime = DateTime(
      2015, 9, 12,
      h, m,
      0, 32, 0.0);
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

  byte hDesena = h / 10;
  byte hUnitat = h % 10;
  byte mDesena = m / 10;
  byte mUnitat = m % 10;
  
  // Print the hour (blink if setting hour)
  if (timeMode == TIME_MODE_NORMAL ||
      timeMode == TIME_MODE_SETTING_HOUR && blink || 
      timeMode == TIME_MODE_SETTING_MINUTE)
  {
    addCharToFrame(0           ,0            , hDesena);
    addCharToFrame(CHAR_WIDTH+1,0            , hUnitat);
  }
  
  // Print the minute (blink if setting minute)
  if (timeMode == TIME_MODE_NORMAL ||
      timeMode == TIME_MODE_SETTING_HOUR ||
      timeMode == TIME_MODE_SETTING_MINUTE && blink)
  {
    addCharToFrame(0           ,CHAR_HEIGHT+1, mDesena);
    addCharToFrame(CHAR_WIDTH+1,CHAR_HEIGHT+1, mUnitat);
  }
  
  // Draw blinking colon
  //frame[2][13] = blink;
  //frame[4][13] = blink;  
  setFrame(frame2,2,13,blink);
  setFrame(frame2,4,13,blink);
  blink = !blink;

  updateDisplay();
}



// Show the current time on the display using words
void showTimeWords()
{
  byte h2 = h;

  // Minuts
  if (m == 0) {
    afegirEsSon(h,m);
    afegirLaLesHores(h);
    addWordToFrame(w_h_en);
    addWordToFrame(w_h_punt); 
    
  } else { //Minuts són diferents de 0
    if (m < 7) {
      afegirEsSon(h,m);
      afegirLaLesHores(h);
      addWordToFrame(w_i_p_min);
      addWordToFrame(w_minuts_passen[m - 1]);
      if (m == 1) {  
        addWordToFrame(w_p_minut);
      } else {
        addWordToFrame(w_p_minuts);
      }
    } else {
      h2++; // Hora següent
      if (m == 7) {
        afegirEsSon(h2,m);
        addWordToFrame(w_mig_q);
        addWordToFrame(w_quart);   
        afegirDpelsQuarts(h2);
      } else if (m >= 8 && m <= 14) {   // Falten minuts per un quart
        if (m == 14) {
          addWordToFrame(w_falta);
          addWordToFrame(w_f_minut); 
        } else {
          addWordToFrame(w_falten);
          addWordToFrame(w_f_minuts); 
        }
        addWordToFrame(w_minuts_falten[m-8]);
        addWordToFrame(w_per);
        addWordToFrame(w_a);    
        addWordToFrame(w_un_q);
        addWordToFrame(w_quart);
        afegirDpelsQuarts(h2); 
      } else if (m == 15) {              // Un quart
        afegirEsSon(h2,m);
        addWordToFrame(w_un_q);
        addWordToFrame(w_quart);
        afegirDpelsQuarts(h2);  
      } else if (m >= 16 && m <= 21) {    // Un quart i
        afegirEsSon(h2,m);
        addWordToFrame(w_un_q);
        addWordToFrame(w_quart);
        afegirDpelsQuarts(h2);
        addWordToFrame(w_i_p_min);
        addWordToFrame(w_minuts_passen[m-16]);
        if (m == 16) {
          addWordToFrame(w_p_minut); 
        } else {
          addWordToFrame(w_p_minuts); 
        }
      } else if (m == 22) {              // Un quart i mig
        afegirEsSon(h2,m);
        addWordToFrame(w_un_q);
        addWordToFrame(w_quart);
        addWordToFrame(w_im);
        addWordToFrame(w_mig);
        afegirDpelsQuarts(h2); 
      } else if (m >= 23 && m <= 29) {  // Falten minuts per dos quarts
        if (m == 29) {
          addWordToFrame(w_falta);
          addWordToFrame(w_f_minut); 
        } else {
          addWordToFrame(w_falten);
          addWordToFrame(w_f_minuts); 
        }
        addWordToFrame(w_minuts_falten[m-23]);
        addWordToFrame(w_per);
        addWordToFrame(w_a);  
        addWordToFrame(w_dos_q);
        addWordToFrame(w_quarts); 
        afegirDpelsQuarts(h2); 
      } else if (m == 30) {              // Dos quarts
        afegirEsSon(h2,m);
        addWordToFrame(w_dos_q);
        addWordToFrame(w_quarts); 
        afegirDpelsQuarts(h2);
      } else if (m >= 31 && m <= 36) {   // Dos quarts i
        afegirEsSon(h2,m);
        addWordToFrame(w_dos_q);
        addWordToFrame(w_quarts);
        afegirDpelsQuarts(h2);
        addWordToFrame(w_i_p_min);
        addWordToFrame(w_minuts_passen[m-31]);
        if (m == 31) {
          addWordToFrame(w_p_minut); 
        } else {
          addWordToFrame(w_p_minuts); 
        }
      } else if (m == 37) {
        afegirEsSon(h2,m);
        addWordToFrame(w_dos_q);
        addWordToFrame(w_quarts);
        addWordToFrame(w_im);
        addWordToFrame(w_mig);
        afegirDpelsQuarts(h2);
      } else if (m >= 38 && m <= 44) { // Falten minuts per tres quarts
        if (m == 44) {
          addWordToFrame(w_falta);
          addWordToFrame(w_f_minut); 
        } else {
          addWordToFrame(w_falten);
          addWordToFrame(w_f_minuts); 
        }
        addWordToFrame(w_minuts_falten[m-38]);
        addWordToFrame(w_per);
        addWordToFrame(w_a); 
        addWordToFrame(w_tres_q);
        addWordToFrame(w_quarts);
        afegirDpelsQuarts(h2);  
      } else if (m == 45) {            // Tres quarts
        afegirEsSon(h2,m);
        addWordToFrame(w_tres_q);
        addWordToFrame(w_quarts);
        afegirDpelsQuarts(h2);
      } else if (m >= 46 && m <= 51) { // Tres quarts i
        afegirEsSon(h2,m);
        addWordToFrame(w_tres_q);
        addWordToFrame(w_quarts);
        afegirDpelsQuarts(h2);
        addWordToFrame(w_i_p_min);
        addWordToFrame(w_minuts_passen[m-46]);
        if (m == 46) {
          addWordToFrame(w_p_minut); 
        } else {
          addWordToFrame(w_p_minuts); 
        }
      } else if (m == 52) {
        afegirEsSon(h2,m);
        addWordToFrame(w_tres_q);
        addWordToFrame(w_quarts);
        addWordToFrame(w_im);
        addWordToFrame(w_mig);
        afegirDpelsQuarts(h2);
      } else if (m >= 53 && m <= 59) {
        if (m == 59) {
          addWordToFrame(w_falta);
          addWordToFrame(w_f_minut); 
        } else {
          addWordToFrame(w_falten);
          addWordToFrame(w_f_minuts); 
        }
        addWordToFrame(w_minuts_falten[m-53]);
        addWordToFrame(w_per);
        addWordToFrame(w_a); 
        afegirLaLesHores(h2);
      }
    }
  } 
  
  // Hores
  if (h2 == 0) {
    addWordToFrame(w_hores[11]);
  } else if (h2 <= 12) {
    addWordToFrame(w_hores[h2 - 1]);
  } else {
    addWordToFrame(w_hores[h2 - 13]);
  }
  // Matí, migdia, tarda o nit
  if (h >= 6 && h < 12) {
    addWordToFrame(w_del);
    addWordToFrame(w_mati);
  } else if (h >= 12 && h < 15) {
    addWordToFrame(w_del);
    addWordToFrame(w_migdia);
  } else if (h >= 15 && h < 21) {
    addWordToFrame(w_boot_de);
    addWordToFrame(w_boot_la); 
    addWordToFrame(w_tarda);
  } else {
    addWordToFrame(w_boot_de);
    addWordToFrame(w_boot_la);
    addWordToFrame(w_nit);
  }

  updateDisplay();
}

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
    if (minut >= 0 && minut <= 7 || minut >= 15 && minut <= 22) //És entre la una en punt i mig quart / És entre un quart i un quart i mig
      addWordToFrame(w_es);
    else														//Són dos quarts / Són dos quarts i mig / Són tres quarts / Són tres quarts i mig
      addWordToFrame(w_son);
  } 
  else 
  {
    if (minut == 7 || minut >= 15 && minut <= 22)				// És mig quart / És entre un quart i  un quart i mig
      addWordToFrame(w_es);
    else
      addWordToFrame(w_son);									//Són les dues en punt / Són dos quarts / Són dos quarts i mig / Són tres quarts / Són tres quarts i mig
  }
}


