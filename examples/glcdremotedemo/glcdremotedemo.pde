#include <GLCD_proxy.h>
#include <Ports.h>
#include <RF12.h>

GLCD_proxy glcd (2); // ID of remote LCD host

void setup () {
//    Serial.begin(57600);
//    Serial.println("Remote LCD test");

// the following lines is the only addition to a locally running glcd sketch
    rf12_initialize(1, RF12_868MHZ, 212);
    
    glcd.begin();
    glcd.display();    
}

void loop () {
    delay(100);
    glcd.clear();
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
      glcd.display();
    }
    delay(2000);
    glcd.clear();
    glcd.drawstring(8,4,"Is that snow?");
    glcd.display();
    delay(2000);
    for (int i=0;i<1000;i++) {
      glcd.setpixel(random(LCDWIDTH)-1,random(LCDHEIGHT)-1,1);
      glcd.display();
      if (i%60==0) {
        glcd.scrolldown(8);
        delay(10);
      }
    }
    delay(2000);
    glcd.clear();
    glcd.display();
    glcd.drawstringx(0,30,"Some");
    delay(50);
    glcd.drawstringx(60,30,"text");
    delay(50);
    glcd.display();
    delay(2000);
    glcd.drawstringx(27,27,"wonky");
    glcd.display();
    delay(1000);
    
    char *wonky="wonky";
    for (byte i=0;i<5;i++) {
      glcd.drawcharx(27+i*6,20-i,wonky[i]);
      delay(50);
      glcd.display();
    }
    delay(2000);

    glcd.clear();  

    glcd.drawstring(8,4,"Rectangle?");
    glcd.display();
    delay(2000);
    glcd.drawrect(2,2,64,50,1);    
    glcd.display();
    delay(2000);
    for (byte x=1;x<20;x=x+3) {
      glcd.fillrect(40+x,10+x,64-(2*x),50-(2*x),x&1);    
      delay(100);
      glcd.display();
    }
    delay(2000);
    glcd.clear();
    
    glcd.drawstring(8,4,"circle?");
    glcd.display();
    delay(2000);
    glcd.drawcircle(63,31,31,1);    
    glcd.display();
    delay(2000);
    for (byte x=1;x<20;x=x+3) {
      glcd.fillcircle(80-x,31,31-x,x&1);    
      delay(100);
      glcd.display();
    }
    delay(2000);
    glcd.clear();

    glcd.drawstring(8,4,"Triangle?");
    glcd.display();
    delay(2000);
    glcd.drawtriangle(0,0,30,63,80,20,1);
    glcd.display();
    delay(2000);
    for (byte x=1;x<20;x=x+3) {
      glcd.filltriangle(40+x,x,50+x,63-x,120-x,20+x,x&1);
      delay(100);
      glcd.display();
    }

    delay(2000);    
}
