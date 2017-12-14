#ifndef CREDITS_H
#define CREDITS_H
#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define DELAY_AFTER_LETTER  500
#define DELAY_AFTER_NAME   3000


// Boot message words
const byte   w_boot_josep[3] PROGMEM = { 3,  2, 5 };
const byte       w_boot_i[3] PROGMEM = { 3,  8, 1 };
const byte     w_boot_ana[3] PROGMEM = { 3, 10, 3 };
const byte    w_boot_dels[3] PROGMEM = { 13, 1, 4 };
const byte      w_boot_de[3] PROGMEM = { 13, 6, 2 };
const byte      w_boot_la[3] PROGMEM = { 13, 9, 2 };
const byte     w_boot_uni[3] PROGMEM = { 13,12, 3 };
const byte      w_boot_12[3] PROGMEM = { 14, 4, 2 };
const byte     w_boot_set[3] PROGMEM = { 14, 7, 3 };
const byte    w_boot_2015[3] PROGMEM = { 14,11, 4 };

// Show the boot message



//void showBootMessage();


#define NUM_NAMES 1 // SET TO NUMBER OF NAMES IN THE LIST

const byte numLettersByName[] PROGMEM =
{
 
  32, //BRETTOLIVERWORDCLOCK2017
  3, //BST note number is size of word
  3, //GMT
  5, //Cesar
  5, //Enric
  6, //Isabel
  5, //Jordi
  5, //Laura
  5, //Oriol
  5  //Sergi 
};

 
 
  
const byte letters[] PROGMEM =
{
 
   // 38BRETTOLIVERWORDCLOCK2017  Title Note each letter co-ord version number shows first on bottom row
    15,3, //version number 3
    15,14, //version number 8
    15,14, //version number 8 // repeat last letter of each word twice to give a pause between words
    15,14, //version number 8
    0, 3, 
    0, 8,
    0,11,
    1,13,
    1,14,
    1,14, // repeat T of Brett once short pause
    2,12,
    2,15,
    3, 9,
    3,10,
    3,11,
    3,12,
    3,12, // repeat R of OLIVER twice long pause
    3,12, // repeat R of OLIVER twice long pause
    7,13,
    7,14,
    7,15,
   10,15,
   10,15, // repeat D of WORD once short pause
    11,1,
    11,2,
    11,3,
    11,4,
    11,5,
   11,12,
   11,13,
   11,14,
   11,15,
  
 /*    //BST
   0,3,
   2,3,
   2,6,
  

  //GMT
   3, 2,
   7, 0,
   7, 4,
 */
};


void creditsManager(unsigned long now, boolean modeChanged);

void setupCreditsNames();

void iterateCreditsNames();

#endif


