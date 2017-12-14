
/*
 * 
 * 
 * 
~~~~~~~~~~~~~~~~~~~ English Word Clock by Brett Oliver ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Based on the original software and hardware Wordclock design by Wouter Devinck (August 2014)
                                 
and the Software design based on the Pijuana Catalan Wordclock by
Aleix Mercader, Antonio Camacho, Cesar August Pomerol, Enric Masdeu,Isabel Martínez,
Jordi Aranda, Laura Mampel, Oriol Bernadó, Sergi Vaqué
######################################

version number on credits on start  (credit.h line 47)

#####################################


v1.0 Basic setup no code changes
v1.1 Adding English to Time.cpp replaced Unitat with Units & Desena with Tenths
     Time.h showTimeWords() English added 
v1.2 Time.h changing words to match English positions on matrix
v1.4 Working English time. temperature not working
v1.5 Add countdown to sync and removed temperature in word clock display sync on 30 working
v1.6 Add seconds counter and minutes when clock says 6 past 3 becomes 6 minutes past 3
v1.7 Disabled boot message Add credits "Brett Oliver Word Clock 2017"
v1.8 Add Sync display in Word Clock to show when 30 second sync has arrived
v1.9 Add full seconds to bottom row Add PIR ON OFF on display only
v2.0 Changed time the clock says night/morning fixed bug where seconds did not show at 12:00 or 24:00 on word clock
     A few minor changes to wording at midday and midnight
v2.1 Added software version number on credits on start  (credit.h line 47)
v2.2 Added the word SIMON to simon.cpp as letters have changed to English vrsion
v2.3 PIR sensor added 
v2.4 PIR On/Off control from Word Menu Bot Left Button On Bott Right Off
     PIR On shows when PIR enable & PIR Off when disabled (note when disabled display is on all the time)
v2.5 Added seconds to Digital and Analogue time
v2.6 Added delay after each word in credits
v2.7 Clock syncs on 30 second pulse and shows SYNC until seconds reach 36
v2.8 PIR ON & PIR OFF displayed for a few seconds only
v2.9 PIR On or Off display is turen off at 0 seconds in case buttin was pressed after 54 seconds. 
v3.0 Changed 30 sec sync as was syncing on 30 sec and 0 sec. On 30 second sync sunc is diabled untill seconds reach 10. Secs set back to 30 sec on sync
     time.cpp line 802 Sync displayed at 30 sec only rather than until 36
v3.1  Added reset seconds to zeroe in clock set mode Bott right button pressed
v3.2 Light readings adjusted brightness.cpp line 32 
v3.3 Sync @s>20 and s<40  Simon title changed
v4.3 Not working
v3.5  Added auto summer/winter change on Digital pin 7 from Pragotron v2.5 and above Mster Clock
v3.6 Corrected missing Evening from 18:00hrs to 20:59hrs
v3.7 Found "THE TIME IS" was corrupting every now and then. Modified time.cpp to refresh "THE TIME IS" on every time computation
v3.8 Correct switches in tetris. Modified by swapping TL,TR,BL & BR key up and key down in tetris.cpp(this could be due to the TTP223 Touch Sensor used on this version)
stc.cpp
 Process a key down event  588
 Process a key up event 622

 tetris.cpp
 modify this to read controller inputs 217

 

 


Brightness displayed in credit screen not done yet.

*/
// Pijuana Wordclock Project firmware
// ==================
// Based on the Wordclock by Wouter Devinck (August 2014)
// Translation and extension of that project, adding Catalan hours format and many new modes.
// July 2015 - September 2015
 
// Aleix Mercader, Antonio Camacho, Cesar August Pomerol, Enric Masdeu,
// Isabel Martínez, Jordi Aranda, Laura Mampel, Oriol Bernadó, Sergi Vaqué

// Dependencies:
//  * Arduino libraries                - http://arduino.cc/
//  * Chronodot library (for DS3231)   - https://github.com/Stephanie-Maks/Arduino-Chronodot
//  * LedControl library (for MAX7219) - http://playground.arduino.cc/Main/LedControl
//  * Temp. sensor library (for DHT22) - https://github.com/adafruit/DHT-sensor-library
//  * Simple Tetris Clone library      - https://github.com/ex/blocks

/* Hardware block diagram:

              +-----------------+
              | Real time clock |
              | Maxim DS3231    |
              +--------+--------+
                       |I2C
         +-------------+-------------+
+-----+  |                           |   +------------------+
|DHT22+--|                           +---+ 8x8 LED matrix 1 |
+-----+  |                           |   | Maxim MAX7219    |
         |                           |   +---------+--------+
+---+    |      Microcontroller      |             |
|LDR+----+      Atmel ATMEGA328 NANO |   +---------+--------+
+---+    |      (with Arduino        |   | 8x8 LED matrix 2 |
         |       bootloader)         |   | Maxim MAX7219    |
+------+ |                           |   +---------+--------+
|Buzzer+-+                           |             |
+------+ |                           |   +---------+--------+
         +-++----++---------++----++-+   | 8x8 LED matrix 3 |
           ||    ||         ||    ||     | Maxim MAX7219    |
    +------++-+  ||  +------++-+  ||     +---------+--------+
    |         |  ||  |         |  ||               |
    | TTP223B |  ||  | TTP223B |  ||     +---------+--------+
    +---------+  ||  +---------+  ||     | 8x8 LED matrix 4 |
                 ||               ||     | Maxim MAX7219    |
          +------++-+      +------++-+   +------------------+
          |         |      |         |
          | TTP223B |      | TTP223B |
          +---------+      +---------+

(created using http://asciiflow.com/) */


// Includes
#include <EEPROM.h>
#include <Wire.h>


#include "brightness.h"
#include "character.h"
#include "credits.h"
#include "display.h"
#include "life.h"
#include "serial.h"
#include "simon.h"
#include "temphum.h"
#include "tetris.h"
#include "time.h"
#include "LedControl.h"


#include "timeanalog.h"
#include "touchbuttons.h"

//Brett
int synccheck = 0;
int syncIP = 5; // Sync input D5
int PIR = 8; // PIR input 8 Pin14 on IC
int PIRSw = 1; // PIR control is Off on startup (display is always On)

//Brett
int BSTin = 7; // Input from Master clock to indicate BST/GMT. BSTin == 1 = BST and  BSTin == 0 = GMT
int BSTcheck = 0; // 1 when BST On and 0 when BST off 


//Brett Summer Wnter Auto change




////Brett Summer Wnter Auto change


void setup() {
  
//Brett

  pinMode(syncIP, INPUT);
  pinMode(PIR, INPUT);
  pinMode(BSTin, INPUT);
  
  //Brett 


  // Debug info
  Serial.begin(9600);
  Serial.println(F("Wordclock is booting"));
  
  // Initiate the LED drivers
  // Brightness is set to max by default
  Serial.println(F("Init LED drivers"));
  initDisplay();

  // Initiate the Real Time Clock
  Serial.println(F("Init RTC"));
  initRTC();

  // Initiate the temperature and humidity sensor
  Serial.println(F("Init temp sensor"));
  initTempHum();

  // Initiate the capacitive touch inputs
  Serial.println(F("Init touch sensor"));
  initTouch();

  // Read settings from EEPROM
  Serial.println(F("Read settings"));
  byte initial = EEPROM.read(EEPROM_INITIAL_MODE);
  boolean showBoot = EEPROM.read(EEPROM_SHOW_BOOT);

  setBrightness(15);
  
  // Boot message
  if (showBoot)
 //   showBootMessage();
  
  Serial.println(F("Done. Hello!"));

  // Show serial menu
  printMenu();

  // Go to initial mode
  goToMode(initial);
}


void loop() {


//Brett BST auto set

if ( digitalRead (BSTin) == HIGH && BSTcheck == 0 )
{ 

 BSTcheck = 1;
  h = h+1; // adds an hour to the time
  
   updateRTCtime();
}

if ( digitalRead (BSTin) == LOW && BSTcheck == 1 )
{ 
 
  
  h = h-1; // takes an hour off the time
  BSTcheck = 0;
  
   updateRTCtime();
}

//brett



//brett
//Brett BST auto set end


//Brett Synchronisation
  
 if ( digitalRead (syncIP) == HIGH && synccheck == 0 && s > 20 && s < 40)//if sync pulse is high and not sync'd in the last minute and seconds >20 & <40
  {
  synccheck = 1; // sync is turned off 
  //Serial.println(synccheck);
  s = 30; // seconds are reset to 30 
  updateRTCtime(); //see time.cpp
 // Serial.println(s);
  
  }
  
  if ( s == 10 )// at 10 seconds sync is re-enabled
  {
  synccheck = 0; // 
  //Serial.println(synccheck);
 
  
  }
  //Brett

//Brett PIR Sense
  
 if  ( PIRSw == 1 )
  {
      initDisplayOn();// Turns on Display all the time when PIRSw = 1 (PIR disabled) 
    }
 
else if ( digitalRead ( PIR) == LOW && PIRSw == 0) // If PIR not triggered and PIRSw has not disabled the PIR 
  {
         initDisplayOff(); // Turns the display off see display.cpp
  }
  
   else if  ( digitalRead (PIR) == HIGH && PIRSw == 0 ) // If PIR triggered and PIRSw has not disabled the PIR 
    {
      initDisplayOn();// The display is turned on see display.cpp
    }
   
  //Brett


    //Brett
   // Turns  PIR control of the display On and Off Bott Left PIR control enabled
   // Bott Right PIR control disabled and the display is always on (default on power up)
if (displayMode == MODE_WORD_CLOCK) // Allow PIR On/Off only in word clock mode


{
    if (BL_KEY_DOWN) // Bottom left button pressed
    {
     PIRSw = 0;
     displayPIRon(); // Turns on PIR On on display see time.cpp
     // Serial.println("PIR On");
     
      }
      else if (BR_KEY_DOWN) // Bottom left button pressed
      {
        PIRSw = 1;
        displayPIRoff(); // Turns on PIR Off on display see time.cpp
     
    //  Serial.println("PIR Off");
      }  

}



  // Camacho's state machine
  //getInput();
  readTouch();
  stateManager();
  //setOutput();
}



void stateManager()
{
  static byte prevDisplayMode = -1;
  
  unsigned long now = millis();

  boolean modeChanged = prevDisplayMode != displayMode;
  prevDisplayMode = displayMode;

  // Run state-dependent code
  switch (displayMode)
  {
    case MODE_WORD_CLOCK:
    case MODE_DIGITAL_CLOCK:
    case MODE_ANALOG_CLOCK:

      timeManager(now, modeChanged);
      break;

    case MODE_TEMP_HUMIDITY:

      tempHumManager(now, modeChanged);
      break;

    case MODE_GAME_OF_LIFE:
    
      gameOfLifeManager(now, modeChanged);
      break;

    case MODE_SIMON:
    
      simonManager(now, modeChanged);
      break;

    case MODE_TETRIS:
      
      tetrisManager(now, modeChanged);
      break;
      
    case MODE_CREDITS:

      creditsManager(now, modeChanged);
      break;
      
    default:
      break;
  }

  // State-independent code
  brightnessManager(now);
  serialManager(now);
}
