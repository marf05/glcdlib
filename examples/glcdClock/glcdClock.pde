// Demo clock for the Graphics Board, this version runs on the internal clock.
// 2010-11-18 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id$

#include <GLCD_ST7565.h>
#include <RTClib.h>
#include <Wire.h> // needed to avoid a linker error :(
#include <avr/pgmspace.h>
#include "digits.h"

GLCD_ST7565 glcd(14, 4, 17, 7);
RTC_Millis rtc;

static void drawDigit(byte x, byte d) {
    const long* digit = digits + 64 * d;
    for (byte i = 0; i < 64; ++i) {
        long mask = pgm_read_dword(digit++);
        for (byte j = 0; j < 28; ++j)
            glcd.setpixel(x + j, i, bitRead(mask, 27-j));
    }
}

static void twoDigits(byte x, byte v) {
    drawDigit(x, v / 10);
    drawDigit(x + 32, v % 10);
}

void setup () {
    // Serial.begin(57600);
    // Serial.println("\n[glcdClock]");
    
    rtc.begin(DateTime (__DATE__, __TIME__));
    
    glcd.st7565_init();
    glcd.st7565_command(CMD_DISPLAY_ON);
    glcd.st7565_command(CMD_SET_ALLPTS_NORMAL);
    glcd.st7565_set_brightness(0x15);
    glcd.clear();

    drawDigit(0, 1);

    glcd.display();
}

void loop () {
    DateTime now = rtc.now();
    // hours
    twoDigits(0, now.hour());
    // minutes
    twoDigits(69, now.minute());
    // blinking colon
    glcd.fillcircle(63, 24, 2, now.second() & 1);
    glcd.fillcircle(63, 40, 2, now.second() & 1);
    // show it!
    glcd.display();
}
