#include "GLCD_ST7565.h"
#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(

GLCD_ST7565 glcd;

byte oldx=0;
byte oldy=0;
int xlow=110;
int xhigh=800;
int ylow=210;
int yhigh=800;
int xpos=-1;
int ypos=-1;

Port xport (2);
Port yport (3);

void GetTouchPos(byte raw=0){
  xport.mode(OUTPUT);
  xport.mode2(OUTPUT);
  xport.digiWrite2(LOW);
  xport.digiWrite(HIGH);
 
  yport.digiWrite(LOW);
  yport.digiWrite2(LOW);
 
  yport.mode(INPUT);
  yport.mode2(INPUT); 
  delay(10); // let the analogue port make it's sample
 
  //xLow has analog port -14 !!
  xpos=yport.anaRead();
  
  yport.mode(OUTPUT);
  yport.mode2(OUTPUT);
  yport.digiWrite2(LOW);
  yport.digiWrite(HIGH);
 
  xport.digiWrite(LOW);
  xport.digiWrite2(LOW);
 
  xport.mode(INPUT);
  xport.mode2(INPUT);
  delay(10); // let the analogue port make it's sample
 
  //xLow has analog port -14 !!
  ypos=xport.anaRead();
  if ((xpos!=0) && (ypos!=0)) {
    if (raw==0) {
/*
      char t[6];
      glcd.fillrect(0,0,20,16,0);
      itoa(xpos,t,10);
      glcd.drawstringx(0,0,t);
      itoa(ypos,t,10);
      glcd.drawstringx(0,8,t);
      glcd.display();
*/

      xpos=xpos-xlow;
      ypos=ypos-ylow;
      if (xpos<0) xpos=-1;
      else {
        long range=xhigh-xlow;
        range=12800/range;
        long temp=(xpos*range)/100;
        xpos=int(temp);
      }
      
      if (ypos<0) ypos=-1;
      else {
        long range=yhigh-ylow;
        range=6400/range;
        long temp=(ypos*range)/100;
        ypos=int(temp);
      }
/*
      glcd.fillrect(100,0,20,16,0);
      itoa(xpos,t,10);
      glcd.drawstringx(100,0,t);
      itoa(ypos,t,10);
      glcd.drawstringx(100,8,t);
      glcd.display();
*/
    }
  }
  else {
    xpos=-1;
    ypos=-1;
  }
}

void CalibrateTouch(){
  glcd.clear();  
  glcd.drawstringx(3,25,"Touch the two corner");
  glcd.drawstringx(3,33,"points when they are");
  glcd.drawstringx(36,41,"indicated");
  glcd.drawtriangle(2,2,8,2,2,8,1);
  glcd.drawline(5,5,8,8,1);
  glcd.display();

  GetTouchPos(1);
  while ((xpos==-1) || (ypos==-1)){
    glcd.setpixel(0,0,1);
    glcd.display();
    for (byte i=0;i<50;i++) {
      GetTouchPos(1);
      if (xpos!=-1) break;
    }
    if (xpos==-1) {
      glcd.setpixel(0,0,0);
      glcd.display();
      for (byte i=0;i<50;i++) {
        GetTouchPos(1);
        if (xpos!=-1) break;
      }
    }
  }
  int tempxlow=xpos;
  int tempylow=ypos;
  while (xpos!=-1) GetTouchPos(1);
  glcd.clear();
  glcd.drawtriangle(LCDWIDTH-3,LCDHEIGHT-3,LCDWIDTH-9,LCDHEIGHT-3,LCDWIDTH-3,LCDHEIGHT-9,1);
  glcd.drawline(LCDWIDTH-6,LCDHEIGHT-6,LCDWIDTH-9,LCDHEIGHT-9,1);

  glcd.display();
  
  while ((xpos==-1) || (ypos==-1)){
    glcd.setpixel(LCDWIDTH-1,LCDHEIGHT-1,1);
    glcd.display();
    for (byte i=0;i<50;i++) {
      GetTouchPos(1);
      if (xpos!=-1) break;
    }
    if (xpos==-1) {
      glcd.setpixel(LCDWIDTH-1,LCDHEIGHT-1,0);
      glcd.display();
      for (byte i=0;i<50;i++) {
        GetTouchPos(1);
        if (xpos!=-1) break;
      }
    }
  }
  xlow=tempxlow;
  ylow=tempylow;  
  xhigh=xpos;
  yhigh=ypos;
  glcd.clear();
/*
  char t[6];
  itoa(xlow,t,10);
  glcd.drawstringx(0,0,t);
  itoa(xhigh,t,10);
  glcd.drawstringx(40,0,t);
  itoa(ylow,t,10);
  glcd.drawstringx(0,8,t);
  itoa(yhigh,t,10);
  glcd.drawstringx(40,8,t);
*/
  glcd.drawstringx(25,20,"Thank you");
  glcd.drawstringx(3,28,"Press OK to continue");
  glcd.drawstringx(52,42,"OK");
  glcd.drawrect(50,40,15,11,1);
  glcd.display();  

  while ((xpos<50) || (xpos>65) || (ypos<40) || (ypos>51)) {
    GetTouchPos();
  }
  glcd.clear();
  glcd.display();
  while (xpos!=-1) GetTouchPos();  
}

void setup(){
  glcd.begin();
  CalibrateTouch();
  glcd.clear();
  glcd.display();
}
 
void loop(){
  GetTouchPos();
  if (xpos!=-1) {
    glcd.setpixel(xpos,ypos,1);
    glcd.display();
  }
}

