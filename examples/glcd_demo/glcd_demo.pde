// Demo display for the Graphics Boad
// 2010-11-14 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id$

#include "GLCD_ST7565.h"
#include <Ports.h>
#include <RF12.h> // needed to avoid a linker error :(
#include <avr/pgmspace.h>

GLCD_ST7565 glcd;

void setup () {
    Serial.begin(57600);
    Serial.println("\n[glcd_demo]");
    rf12_initialize(1, RF12_868MHZ);
    rf12_sleep(0);
    
    glcd.begin();
#if 1    
    pinMode(3, OUTPUT);
    digitalWrite(3, 1); // turn the backlight on
#endif

    // draw a string at a location, use _p variant to reduce RAM use
    glcd.drawstring_p(40, 0, PSTR("ARDUINO"));
    glcd.drawstring_p(10, 2, PSTR("ST7565 128x64 GLCD"));
    glcd.drawstring_p(22, 4, PSTR("Graphics Board"));
    glcd.drawstring_p(20, 6, PSTR("JeeLabs.org/gb1"));

    glcd.drawcircle(5, 5, 5, WHITE);
    glcd.fillcircle(121, 5, 5, WHITE);
    glcd.fillcircle(6, 58, 5, WHITE);
    glcd.drawcircle(121, 58, 5, WHITE);

    glcd.drawline(40, 9, 81, 9, WHITE);
    glcd.drawline(40, 11, 81, 11, WHITE);
    glcd.drawline(0, 42, 14, 28, WHITE);
    glcd.drawline(112, 42, 126, 28, WHITE);
    glcd.drawrect(0, 28, 127, 15, WHITE);

    glcd.display();
    
    Sleepy::powerDown();
}

void loop () {}
