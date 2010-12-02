#include <ST7565.h>
#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(

ST7565 glcd(14, 4, 17, 7);

int d=4;
byte x=0;

void setup () {
    glcd.st7565_init();
    glcd.st7565_command(CMD_DISPLAY_ON);
    glcd.st7565_command(CMD_SET_ALLPTS_NORMAL);
    glcd.st7565_set_brightness(0x015);
    glcd.clear();
    glcd.display();
}

void loop () {
//  glcd.fillrect(3,10,110,50,1);
  glcd.drawline(0,0,127,63,1);
  glcd.drawstring(x,0,"Hello");
  glcd.display();
  glcd.drawstringx(x+36,2,"Wo");
  glcd.drawcharx(x+48,3,'r');
  glcd.drawcharx(x+54,4,'l');
  glcd.drawstringx(x+60,5,"d!");
  if (((x+d)>55) || ((x+d)<0)) d=-d;
  x=x+d;
  glcd.display();
  delay(1000);
  glcd.scrolldown(8);
  glcd.display();
  delay(1000);
}
