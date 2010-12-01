#include <glcdremote.h>
#include <Ports.h>
#include <RF12.h>

glcdremote glcd(14, 4, 17, 7); // the params here are meaningless to glcdremote, but left to make
                               // it interchangeable.

void setup () {
    Serial.begin(57600);
    Serial.println("Remote LCD test");

// The following lines are the only addition to a locally running glcd sketch
    rf12_initialize(1, RF12_868MHZ, 212);
    delay(1000);
    rf12_recvDone();
    glcd.RemoteLCDhostID=2; // really should make this a procedure call :)
// That's all you need for remote display.   
    
    
    glcd.st7565_init();
    glcd.st7565_command(CMD_DISPLAY_ON);
    glcd.st7565_command(CMD_SET_ALLPTS_NORMAL);
    glcd.st7565_set_brightness(0x015);
    glcd.clear();
    glcd.display();    
}

void loop () {
    glcd.clear();
    delay(2000);

    glcd.drawstring(30,3,"Hello World!");
    glcd.display();
    delay(2000);

    glcd.drawstring(8,4,"Let's draw a line");
    glcd.display();
    delay(2000);
    glcd.drawline(0,40,127,40,1);
    glcd.display();
    delay(2000);
    glcd.drawstring(8,4,"And some black dots");
    glcd.display();
    delay(2000);
    for (int i=0;i<=21;i++) {
      glcd.setpixel(i*6,40,0);
    }
    glcd.display();
    delay(2000);
    glcd.clear();
    glcd.drawstring(8,4,"circle?");
    glcd.display();
    delay(2000);
    glcd.drawcircle(63,31,31,1);    
    glcd.display();
    delay(2000);
    glcd.fillcircle(80,21,21,1);    
    glcd.display();
    delay(2000);
    glcd.clear();
    glcd.drawstring(8,4,"Triangle?");
    glcd.display();
    delay(2000);
    glcd.drawtriangle(0,0,30,63,80,20,1);
    glcd.display();
    delay(2000);
    glcd.filltriangle(127,0,30,63,80,20,1);
    glcd.display();
    delay(2000);    
}
