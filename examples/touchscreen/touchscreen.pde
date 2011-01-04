// Touch screen demonstration program.
// $Id: glcdhost.pde 6525 2010-12-22 22:37:37Z sevans $

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
        if (raw==0) {   // optional raw parameter used to leave the raw values for use by the calibrate routine
/*
            char t[6];
            glcd.fillRect(0,0,20,16,0);
            itoa(xpos,t,10);
            glcd.drawString(0,0,t);
            itoa(ypos,t,10);
            glcd.drawString(0,8,t);
            glcd.refresh();
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
            glcd.fillRect(100,0,20,16,0);
            itoa(xpos,t,10);
            glcd.drawString(100,0,t);
            itoa(ypos,t,10);
            glcd.drawString(100,8,t);
            glcd.refresh();
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
    glcd.drawString(3,25,"Touch the two corner");
    glcd.drawString(3,33,"points when they are");
    glcd.drawString(36,41,"indicated");
    glcd.drawTriangle(2,2,8,2,2,8,1);
    glcd.drawLine(5,5,8,8,1);
    glcd.refresh();

    GetTouchPos(1);
    while ((xpos==-1) || (ypos==-1)){
        glcd.setPixel(0,0,1);
        glcd.refresh();
        for (byte i=0;i<50;i++) {
            GetTouchPos(1);
            if (xpos!=-1) break;
        }
        if (xpos==-1) {
            glcd.setPixel(0,0,0);
            glcd.refresh();
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
    glcd.drawTriangle(LCDWIDTH-3,LCDHEIGHT-3,LCDWIDTH-9,LCDHEIGHT-3,LCDWIDTH-3,LCDHEIGHT-9,1);
    glcd.drawLine(LCDWIDTH-6,LCDHEIGHT-6,LCDWIDTH-9,LCDHEIGHT-9,1);

    glcd.refresh();
  
    while ((xpos==-1) || (ypos==-1)){
        glcd.setPixel(LCDWIDTH-1,LCDHEIGHT-1,1);
        glcd.refresh();
        for (byte i=0;i<50;i++) {
            GetTouchPos(1);
            if (xpos!=-1) break;
        }
        if (xpos==-1) {
            glcd.setPixel(LCDWIDTH-1,LCDHEIGHT-1,0);
            glcd.refresh();
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
    glcd.drawString(0,0,t);
    itoa(xhigh,t,10);
    glcd.drawString(40,0,t);
    itoa(ylow,t,10);
    glcd.drawString(0,8,t);
    itoa(yhigh,t,10);
    glcd.drawString(40,8,t);
*/
    glcd.drawString(25,20,"Thank you");
    glcd.drawString(3,28,"Press OK to continue");
    glcd.drawString(52,42,"OK");
    glcd.drawRect(50,40,15,11,1);
    glcd.refresh();  

    while ((xpos<50) || (xpos>65) || (ypos<40) || (ypos>51))
        GetTouchPos();
    glcd.clear();
    glcd.refresh();
    while (xpos!=-1) GetTouchPos();  
}

void setup(){
    glcd.begin();
    glcd.backLight(255);
    CalibrateTouch();
    glcd.clear();
    glcd.refresh();
}
 
void loop(){
    GetTouchPos();
    if (xpos!=-1) {
        glcd.setPixel(xpos,ypos,1);
        glcd.refresh();
    }
}
