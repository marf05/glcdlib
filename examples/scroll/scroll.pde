/*
$Id: glcdhost.pde 6525 2010-12-22 22:37:37Z sevans $

gLCD scroll example.

*/

#include <GLCD_ST7565.h>
#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(

GLCD_ST7565 glcd;

int d=4;
byte x=0;

void setup () {
    glcd.begin();
    glcd.backLight(255);
    glcd.display();
}

void loop () {
//  glcd.fillRect(3,10,110,50,1);
  glcd.drawLine(0,0,127,63,1);
  glcd.drawString(x,0,"Hello");
  glcd.display();
  glcd.drawStringX(x+36,2,"Wo");
  glcd.drawCharX(x+48,3,'r');
  glcd.drawCharX(x+54,4,'l');
  glcd.drawStringX(x+60,5,"d!");
  if (((x+d)>55) || ((x+d)<0)) d=-d;
  x=x+d;
  glcd.display();
  delay(1000);
  glcd.scroll(SCROLLDOWN,8);
  glcd.display();
  delay(1000);
}
