#include "timeanalog.h"
#include "display.h"
#include "time.h"

void line(int x0, int y0, int x1, int y1){
   int dx = abs(x1-x0);
   int dy = abs(y1-y0);
   int sx = (x0 < x1) ? 1 : -1;
   int sy = (y0 < y1) ? 1 : -1;
   int err = dx-dy;

   while(true){
     //frame[y0][x0] = true;
     setFrame(frame2, y0, x0, true);

     if ((x0==x1) && (y0==y1)) break;
     int e2 = 2*err;
     if (e2 >-dy){ err -= dy; x0  += sx; }
     if (e2 < dx){ err += dx; y0  += sy; }
   }
}


void drawTime(int h,int minutes) {
    float angle = pi - (float)h*pi/6.0f;
    line(7,7,
        round(HOUR_HAND_LENGTH*sin(angle)+7.0f),
        round(HOUR_HAND_LENGTH*cos(angle)+7.0f)
        );

    angle = pi - (float)(minutes/5.0f)*(pi/6.0f);
    line(7,7,
        round(MINUTE_HAND_LENGTH*sin(angle)+7.0f),
        round(MINUTE_HAND_LENGTH*cos(angle)+7.0f)
        );
}

// Show the current time on the display using an analog clock
void showTimeAnalog() {

  // Draw the numbers as points
  setFrame(frame2, 0 , 7, true);  //0
  setFrame(frame2, 1 , 11, true); //5
  setFrame(frame2, 4 , 13, true); //10
  setFrame(frame2, 7 , 14, true); //15
  setFrame(frame2, 10, 13, true); //20
  setFrame(frame2, 13, 11, true); //25
  
  setFrame(frame2, 14, 7, true);  //30
  setFrame(frame2, 13, 3, true);  //35
  setFrame(frame2, 10 , 1, true); //40
  setFrame(frame2, 7 , 0, true);  //45
  setFrame(frame2, 4 , 1, true);  //50
  setFrame(frame2, 1 , 3, true);  //55

  drawTime(h, m);

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
  
  updateDisplay();
}

